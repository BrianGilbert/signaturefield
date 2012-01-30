<?php
/**
 * @file
 * Webform module integration.
 */

/**
 * Implements hook_webform_component().
 */
function signaturefield_webform_component_info() {
  $components = array();
  $components['signaturefield'] = array(
    'label' => t('Signature'),
    'description' => t('Signature Pad signature field.'),
    'features' => array(
      'csv' => FALSE,
      'email' => TRUE,
      'email_address' => FALSE,
      'email_name' => FALSE,
      'required' => TRUE,
      // If this field can be used as a conditional SOURCE. All fields may
      // always be displayed conditionally, regardless of this setting.
      // Defaults to TRUE.
      'conditional' => TRUE,
      'group' => FALSE,
      'attachment' => TRUE,
    ),
    'file' => 'includes/webform.inc',
  );

  return $components;
}

/**
 * Generate the form for editing a component.
 *
 * Create a set of form elements to be displayed on the form for editing this
 * component. Use care naming the form items, as this correlates directly to the
 * database schema. The component "Name" and "Description" fields are added to
 * every component type and are not necessary to specify here (although they
 * may be overridden if desired).
 *
 * @param $component
 *   A Webform component array.
 * @return
 *   An array of form items to be displayed on the edit component page
 */
function _webform_edit_signaturefield($component) {
  $form = array();
  // Disabling the description if not wanted.
  $form['description'] = array();
  // Most options are stored in the "extra" array, which stores any settings
  // unique to a particular component type.
  $form['extra']['color'] = array(
    '#type' => 'textfield',
    '#title' => t('Color'),
    '#default_value' => isset($component['extra']['color']) ? $component['extra']['color'] : '#000',
    '#description' => t('Hex value for pen color.'),
    '#maxlength' => 7,
    '#size' => 7,
    '#required' => FALSE,
  );
  $form['extra']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => isset($component['extra']['width']) ? $component['extra']['width'] : '200',
    '#description' => t('Set signature width in pixels.'),
    '#maxlength' => 3,
    '#size' => 3,
    '#required' => FALSE,
  );
  $form['extra']['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => isset($component['extra']['height']) ? $component['extra']['height'] : '55',
    '#description' => t('Set signature height in pixels.'),
    '#maxlength' => 2,
    '#size' => 2,
    '#required' => FALSE,
  );
  return $form;
}

/**
 * Render a Webform component to be part of a form.
 *
 * @param $component
 *   A Webform component array.
 * @param $value
 *   If editing an existing submission or resuming a draft, this will contain
 *   an array of values to be shown instead of the default in the component
 *   configuration. This value will always be an array, keyed numerically for
 *   each value saved in this field.
 */
function _webform_render_signaturefield($component, $value = NULL) {
  $form_item = array(
    '#type' => 'signaturefield',
    '#title' => $component['name'],
    '#required' => $component['mandatory'],
    '#weight' => $component['weight'],
    '#color' => $component['extra']['color'],
    '#width' => $component['extra']['width'],
    '#height' => $component['extra']['height'],
    '#description' => isset($component['extra']['description']) ? _webform_filter_descriptions($component['extra']['description']) : '',
    '#default_value' => $component['value'],
    '#prefix' => '<div class="form-item webform-component-' . $component['type'] . '" id="webform-component-' . $component['form_key'] . '">',
    '#suffix' => '</div>',
  );

  if (isset($value)) {
    $form_item['#default_value'] = $value[0];
  }

  return $form_item;
}

/**
 * Display the result of a submission for a component.
 *
 * The output of this function will be displayed under the "Results" tab then
 * "Submissions". This should output the saved data in some reasonable manner.
 *
 * @param $component
 *   A Webform component array.
 * @param $value
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database table schema.
 * @param $format
 *   Either 'html' or 'text'. Defines the format that the content should be
 *   returned as. Make sure that returned content is run through check_plain()
 *   or other filtering functions when returning HTML.
 * @return
 *   A renderable element containing at the very least these properties:
 *    - #title
 *    - #weight
 *    - #component
 *    - #format
 *    - #value
 *   Webform also uses #theme_wrappers to output the end result to the user,
 *   which will properly format the label and content for use within an e-mail
 *   (such as wrapping the text) or as HTML (ensuring consistent output).
 */
function _webform_display_signaturefield($component, $value, $format = 'html') {
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#markup' => theme('signaturefield_display', array(
      'data' => $value['0'],
      'settings' => array(
        'color' => $component['extra']['color'],
        'width' => $component['extra']['width'],
        'height' => $component['extra']['height'],
        ),
    )),
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#post_render' => array('webform_element_wrapper'),
    '#component' => $component,
    '#format' => $format,
    '#value' => isset($value[0]) ? $value[0] : '',
  );
}
