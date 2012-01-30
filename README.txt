-- SUMMARY --

SignatureField allows you to collect HTML5 canvas based signatures in webform,
cck and in your custom modules via fapi. It then displays the signature as a png file.

It achieves this using the Signature Pad jQuery plugin from:
http://thomasjbradley.ca/lab/signature-pad


-- REQUIREMENTS --

* Libraries API http://drupal.org/project/libraries

-- INSTALLATION --

* Download Signature Pad and extract contents to libraries/signature-pad
* Install and enable Libraries API module
* Install and enable SignatureField module

-- CONFIGURATION --

* Create a webform, content type or custom form with a signaturefield and
  configure settings per field.

-- EXAMPLE CUSTOM FORM --

function signaturefield_menu() {
  $items = array();

  $items['test'] = array(
    'title' => "test",
    'page callback' => 'drupal_get_form',
    'page arguments' => array('signaturefield_test_form'),
    'access arguments' => array('access content'),
    'type' => MENU_CALLBACK,
  );

  return $items;
}

function signaturefield_test_form($form, &$form_state){
  $form = array();

  $form['test'] = array(
    '#title' => 'Test Form',
    '#type' => 'signaturefield',
    '#color' => '#0f0',
  );
  $form['submit'] = array(
    '#value' => 'Submit',
    '#type' => 'submit',
  );

  if (!empty($form_state['signaturefield']['signature'])){
    $form['test'] = array('#value' => theme('signaturefield_display', $form_state['signaturefield']['signature']));
  }

  return $form;

}

function signaturefield_test_form_submit($form_id, &$form_state){
  $form_state['signaturefield']['signature'] = $form_state['input']['test'];
}

-- CONTACT --

Current maintainers:
* Brian Gilbert (realityloop) - http://drupal.org/user/139189

* Realityloop Pty Ltd
  Helping plan, prepare, implement and deploy Drupal solutions tailored to your needs.
  Visit http://www.realityloop.com for more information.
