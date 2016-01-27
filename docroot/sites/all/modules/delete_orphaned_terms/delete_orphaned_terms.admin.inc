<?php

/**
 * @file
 * Admin page callback file for the delete_orphaned_terms module.
 */

/**
 * The admin menu. (Tab 1)
 */
function delete_orphaned_terms_admin() {
  drupal_add_css(drupal_get_path('module', 'delete_orphaned_terms') . "/delete_orphaned_terms.css", 'module', 'all', FALSE);

  $form['dot_cron']['directions'] = array(
    '#markup' => '<p>' . t('This page controls the settings for automatic pruning. To <em>manually</em> delete orphaned terms, use the <a href="@dot">Orphaned Terms Deletion page</a>.', array('@dot' => url('dot'))) . '</p>',
  );

  $form['dot_cron']['delete_orphaned_terms_enablecron'] = array(
    '#default_value' => _dot_get('delete_orphaned_terms_enablecron'),
    '#title' => '<strong>' . t('Enable automatic pruning') . '</strong>',
    '#type' => 'checkbox',
  );

  $form['dot_cron']['lists'] = array(
    '#description' => t('Use these options to force or prevent automatic pruning of your taxonomy.') . '<br/><small>' . t('[t] indicates a tag based vocabulary.') . '</small><br/><span style="text-decoration: underline; font-weight: bold;">' . t('Warning') . '</span>: ' . t('First save at the bottom of this dialog to apply this setting before testing the pruning.') . '<br/><small>' . t('Ctrl-Click for multi select') . '</small>',
    '#title' => t('Whitelisting and blacklisting of vocabularies'),
    '#type' => 'fieldset',
  );
  $form['dot_cron']['lists']['delete_orphaned_terms_whitelist'] = array(
    '#default_value' => _dot_get('delete_orphaned_terms_whitelist'),
    '#description' => t('Which vocabularies to always prune, even if tag based or blacklisted.'),
    '#multiple' => TRUE,
    '#options' => _delete_orphaned_terms_vocab_names(FALSE, TRUE),
    '#title' => t('Whitelist'),
    '#type' => 'select',
  );
  $form['dot_cron']['lists']['delete_orphaned_terms_blacklist'] = array(
    '#default_value' => _dot_get('delete_orphaned_terms_blacklist'),
    '#description' => t('Which vocabularies to never prune.') . '<br/><em>' . t('Note') . '</em>: ' . t('This has no effect if a same item is already selected in the whitelist.'),
    '#multiple' => TRUE,
    '#options' => $form['dot_cron']['lists']['delete_orphaned_terms_whitelist']['#options'],
    '#title' => t('Blacklist'),
    '#type' => 'select',
  );

  $form['dot_cron']['delete_orphaned_terms_tagsonly'] = array(
    '#default_value' => _dot_get('delete_orphaned_terms_tagsonly'),
    '#description' => t('Only act on terms that are part of vocabularies which are tag based.') . '<br/>
      <em>' . t('Note') . '</em>: ' . t('This condition does not apply to items in the whitelist, which are always pruned.') . '<br/>
      <span style="text-decoration: underline; font-weight: bold;">' . t('Warning') . '</span>: ' . t('Orphaned parent terms in a hierarchical vocabulary will cause child terms to be deleted too, regardless of whether the child terms are orphaned or not.'),
    '#title' => t('Prune tag based vocabularies only'),
    '#type' => 'checkbox',
  );

  $form['dot_cron']['delete_orphaned_terms_removeemptyvoc'] = array(
    '#default_value' => _dot_get('delete_orphaned_terms_removeemptyvoc'),
    '#description' => t('Remove vocabularies altogether that might have become empty after pruning (no matter if tag based or not).') . '<br/><em>' . t('Note') . '</em>: ' . t('This does not apply to vocabularies in the blacklist, which will never be deleted.'),
    '#title' => t('Remove empty vocabularies'),
    '#type' => 'checkbox',
  );

  $form['dot_cron']['failsafe'] = array(
    '#collapsible' => TRUE,
    '#description' => t('Use these options to define upper bounds on how many terms should be automatically deleted.'),
    '#title' => t('Failsafe'),
    '#type' => 'fieldset',
  );
  $form['dot_cron']['failsafe']['delete_orphaned_terms_failsafe'] = array(
    '#default_value' => _dot_get('delete_orphaned_terms_failsafe'),
    '#description' => t('Maximum percentage of tags to delete from any one vocabulary in automatic mode.'),
    '#element_validate' => array('_delete_orphaned_terms_element_validate_percentage'),
    '#field_suffix' => '%',
    '#maxlength' => 3,
    '#size' => 3,
    '#title' => t('Any'),
    '#type' => 'textfield',
  );
  $form['dot_cron']['failsafe']['delete_orphaned_terms_failsafe_all'] = array(
    '#default_value' => _dot_get('delete_orphaned_terms_failsafe_all'),
    '#description' => t('Maximum percentage of tags to delete from all vocabularies combined in automatic mode.'),
    '#element_validate' => array('_delete_orphaned_terms_element_validate_percentage'),
    '#field_suffix' => '%',
    '#maxlength' => 3,
    '#size' => 3,
    '#title' => t('All'),
    '#type' => 'textfield',
  );

  $form['dot_cron']['testing'] = array(
    '#collapsible' => FALSE,
    '#title' => t('Testing'),
    '#type' => 'fieldset',
  );
  $form['dot_cron']['testing']['notice'] = array(
    '#markup' => '<p>' . t('Use the button below to simulate what automatic pruning would do to your taxonomy.') . '</p>',
  );
  $form['dot_cron']['testing']['sim'] = array(
    '#cronman' => TRUE,
    '#sim' => TRUE,
    '#submit' => array('_delete_orphaned_terms'),
    '#type' => 'submit',
    '#value' => t('Simulate cron'),
  );
  $form['dot_cron']['testing']['warning'] = array(
    '#markup' => '<p>' . t('Use the button to delete terms now, based on stored cron settings.') . '</p>',
  );
  $form['dot_cron']['testing']['go'] = array(
    '#cronman' => TRUE,
    '#sim' => FALSE,
    '#submit' => array('_delete_orphaned_terms'),
    '#type' => 'submit',
    '#value' => t('Prune now'),
  );

  return system_settings_form($form);
}

/**
 * The admin menu. (Tab 2)
 */
function delete_orphaned_terms_admin_parameters() {

  $form['dot_param']['delete_orphaned_terms_threshold'] = array(
    '#description' => t('Number of nodes associated to a term below (or equal) to which it is considered as orphaned.'),
    '#default_value' => _dot_get('delete_orphaned_terms_threshold'),
    '#element_validate' => array('_delete_orphaned_terms_element_validate_threshold'),
    '#maxlength' => 1,
    '#size' => 1,
    '#title' => t('Orphan threshold'),
    '#type' => 'textfield',
  );

  return system_settings_form($form);
}


