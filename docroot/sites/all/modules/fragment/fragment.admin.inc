<?php
/**
 * @file fragment.module
 * Contains page callbacks for fragment module.
 */

/**
 * Entity form builder.
 *
 * Should be used with entity_ui_get_form().
 *
 * @param $fragment
 *  A fragment entity object.
 * @param $op
 *  One of 'add' or 'edit'.
 */
function fragment_form($form, &$form_state, $fragment, $op = 'edit') {
  $form['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#default_value' => isset($fragment->title) ? $fragment->title : '',
  );

  $form['link'] = array(
    '#title' => t('Link path'),
    '#description' => t('The path this fragment should link to. This can be an internal Drupal path such as %add-node or an external URL such as %drupal, which in either case may include a query string. Enter %front to link to the front page.', array(
      '%add-node' => 'node/add',
      '%drupal' => 'http://drupal.org',
      '%front' => '<front>',
    )),
    '#type' => 'textfield',
    '#default_value' => isset($fragment->link) ? $fragment->link : '',
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save fragment'),
    '#weight' => 40,
  );

  field_attach_form('fragment', $fragment, $form, $form_state);

  return $form;
}

/**
 * Form API submit callback for the fragment edit form.
 */
function fragment_form_submit(&$form, &$form_state) {
  //dsm($form_state);
  $fragment = entity_ui_form_submit_build_entity($form, $form_state);
  //dsm($fragment);

  // Save the entity.
  $fragment->save();
  // Redirect the the admin UI.
  $form_state['redirect'] = 'admin/content/fragments';
}
