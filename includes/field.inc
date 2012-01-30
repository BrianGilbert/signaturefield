<?php
/**
 * @file
 * Content module integration.
 */

/**
 * Implements hook_field_info().
 *
 * Provides the description of the field.
 */
function signaturefield_field_info() {
  return array(
    // The machine name of the field,
    // no more than 32 characters.
    'signaturefield' => array(
      // The human-readable label of the field that will be
      // seen in the Manage fields screen.
      'label' => t('Signature field'),
      // A description of what type of data the field stores.
      'description' => t('Store a signature in the database.'),
      'default_widget' => 'signaturefield_default',
      'default_formatter' => 'signaturefield',
    ),
  );
}

/**
 * Implements hook_field_validate().
 *
 * This hook gives us a chance to validate content that's in our
 * field. We're really only interested in the $items parameter, since
 * it holds arrays representing content in the field we've defined.
 * We want to verify that the items only contain RGB hex values like
 * this: #RRGGBB. If the item validates, we do nothing. If it doesn't
 * validate, we add our own error notification to the $errors parameter.
 *
 * @see signaturefield_field_widget_error()
 */
function signaturefield_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  foreach ($items as $delta => $item) {
    if (!empty($item['value'])) {
      if (!empty($field['max_length']) && drupal_strlen($item['value']) > $field['max_length']) {
        form_set_error($error_element, t('%name: the value may not be longer than %max characters.', array('%name' => $field['widget']['label'], '%max' => $field['max_length'])));
      }
    }
  }
}

/**
 * Implements hook_field_is_empty().
 *
 * hook_field_is_emtpy() is where Drupal asks us if this field is empty.
 * Return TRUE if it does not contain data, FALSE if it does. This lets
 * the form API flag an error when required fields are empty.
 */
function signaturefield_field_is_empty($item, $field) {
  return empty($item['value']);
}

/**
 * Implements hook_field_formatter_info().
 *
 * We need to tell Drupal that we have two different types of formatters
 * for this field. One will change the text color, and the other will
 * change the background color.
 *
 * @see signaturefield_field_formatter_view()
 */
function field_example_field_formatter_info() {
  return array(
    // The machine name of the formatter.
    'signaturefield_formatter' => array(
      // The human-readable label shown on the Display
      // fields screen.
      'label' => t('Signature image'),
      // An array of the field types this formatter
      // can be used on.
      'field types' => array('signaturefield'),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 *
 * Two formatters are implemented.
 * - field_example_simple_text just outputs markup indicating the color that
 *   was entered and uses an inline style to set the text color to that value.
 * - field_example_color_background does the same but also changes the
 *   background color of div.region-content.
 *
 * @see signaturefield_field_formatter_info()
 */
function signaturefield_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  switch ($display['type']) {
    // This formatter simply outputs the field as text and with a color.
    case 'signaturefield':
      foreach ($items as $delta => $item) {
        $element[$delta] = array(
          // We create a render array to produce the desired markup,
          // "<p style="color: #hexcolor">The color code ... #hexcolor</p>".
          // See theme_html_tag().
          '#type' => 'signaturefield',
          '#default_value' => isset($items[$delta]['value']) ? $items[$delta]['value'] : NULL,
          '#color' => isset($field['color']) ? $field['color'] : '#000',
          '#width' => isset($field['width']) ? $field['width'] : 200,
          '#height' => isset($field['height']) ? $field['height'] : 55,
        );
      }
    }
    break;
  }

  return $element;
}


/**
 * Implements hook_field_widget_info().
 *
 * Three widgets are provided.
 * - A simple text-only widget where the user enters the '#ffffff'.
 * - A 3-textfield widget that gathers the red, green, and blue values
 *   separately.
 * - A farbtastic colorpicker widget that chooses the value graphically.
 *
 * These widget types will eventually show up in hook_field_widget_form,
 * where we will have to flesh them out.
 *
 * @see signaturefield_field_widget_form()
 */
function signaturefield_field_widget_info() {
  return array(
    // The machine name of the widget, no more than 32
    // characters.
    'signaturefield_widget' => array(
      // The human-readable label of the field that will be
      // seen in the Manage fields screen.
      'label' => t('Signature field widget'),
      // An array of the field types this widget can be
      // used with.
      'field types' => array('signaturefield'),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 *
 * hook_widget_form() is where Drupal tells us to create form elements for
 * our field's widget.
 *
 * We provide one of three different forms, depending on the widget type of
 * the Form API item provided.
 *
 * The 'field_example_colorpicker' and 'field_example_text' are essentially
 * the same, but field_example_colorpicker adds a javascript colorpicker
 * helper.
 *
 * field_example_3text displays three text fields, one each for red, green,
 * and blue. However, the field type defines a single text column,
 * rgb, which needs an HTML color spec. Define an element validate
 * handler that converts our r, g, and b fields into a simulated single
 * 'rgb' form element.
 */
function field_example_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $value = isset($items[$delta]['signaturefield']) ? $items[$delta]['signaturefield'] : '';

  $widget = $element;
  $widget['#delta'] = $delta;

  $widget += array(
    '#type' => 'signaturefield',
    '#default_value' => isset($items[$delta]['value']) ? $items[$delta]['value'] : NULL,
    '#color' => isset($field['color']) ? $field['color'] : '#000',
    '#width' => isset($field['width']) ? $field['width'] : 200,
    '#height' => isset($field['height']) ? $field['height'] : 55,
  );

  $element['signaturefield'] = $widget;
  return $element;
}

/**
 * Implements hook_field_widget_error().
 *
 * hook_field_widget_error() lets us figure out what to do with errors
 * we might have generated in hook_field_validate(). Generally, we'll just
 * call form_error().
 *
 * @see signaturefield_field_validate()
 * @see form_error()
 */
function signaturefield_field_widget_error($element, $error, $form, &$form_state) {
  switch ($error['error']) {
    case 'signaturefield_invalid':
      form_error($element, $error['message']);
      break;
  }
}
