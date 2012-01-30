<?php
/**
 * @file
 * System module integration.
 */

/**
 * Implements hook_theme().
 */
function signaturefield_theme() {
  $items = array();

  $items['signaturefield'] = array(
//    'arguments' => array('element' => NULL),
    'template' => 'templates/signaturefield',
    'render element' => 'element',
  );
  $items['signaturefield_display'] = array(
    'variables' => array(
      'data' => NULL,
      'settings' => array(),
    ),
  );

  drupal_alter('signaturefield_theme', $items);

  return $items;
}

/**
 * Implements hook_form_alter().
 */
function signaturefield_form_alter(&$form, &$form_state, $form_id) {
  drupal_alter('signaturefield_form_alter', $form, $form_state, $form_id);
}

/**
 * Implements hook_element_info().
 */
function signaturefield_element_info() {
  $type = array();

  $type['signaturefield'] = array(
    '#input' => TRUE,
    '#after_build' => array('signaturefield_after_build'),
    '#height' => 55,
    '#width' => 200,
    '#color' => '#000',
    '#lineTop' => 35,
    '#theme' => 'signaturefield',
  );

  return $type;
}

function form_type_signaturefield_value($element, $edit = FALSE, $form_state) {

  if (!empty($edit)) {
    // If previous value exists delete original image
    if (!empty($element['#default_value'])) {
      $values = unserialize($element['#default_value']);
      @unlink($values['filepath']);
    }
    return signaturefield_json_to_image($element, $edit);
  }

  return $edit;
}

/**
 *
 */
function signaturefield_after_build($element, $form_state) {
  $libpath = libraries_get_path('signature-pad');
  $modpath = drupal_get_path('module', 'signaturefield');

  drupal_add_html_head("<!--[if gte IE 7]><script type='text/javascript' src='{$libpath}/assets/excanvas-r71.min.js'></script><![endif]-->
  <!--[if IE 6]><script type='text/javascript' src='{$libpath}/assets/excanvas-r3.min.js'></script><![endif]-->", $key = NULL);

  drupal_add_js("{$libpath}/build/jquery.signaturepad.min.js");
  drupal_add_js("{$libpath}/build/json2.min.js");
  drupal_add_js("{$modpath}/signaturefield.js");

  drupal_add_js(array(
    'signaturefield' => array(
      'settings' => array(
        'drawOnly' => TRUE,
        'validateFields' => FALSE,
        'penColour' => $element['#color'],
        'lineTop' => $element['#height'] * .85,
      ),
    ),
  ), 'setting');

  drupal_add_css("{$modpath}/signaturefield.css");

  return $element;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function signaturefield_preprocess_signaturefield(&$vars) {
  if (isset($vars['element']['#default_value'])) {
    $values = unserialize($vars['element']['#default_value']);
    $vars['element']['#default_value'] = $values['json'];
    drupal_add_js(array('signaturefield' => array('sig' => $values['json'])), 'setting');
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_signaturefield_display($vars) {
  $values = unserialize(is_array($vars) && !empty($vars['data']) ? $vars['data'] : $vars);
  return theme('image', array('path' => $values['filepath']));
}
