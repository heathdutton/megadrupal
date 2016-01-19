<?php

function librejs_core_js() {
  return array(
    'misc/ajax.js' => '',
    'misc/authorize.js' => '',
    'misc/autocomplete.js' => '',
    'misc/batch.js' => '',
    'misc/collapse.js' => '',
    'misc/drupal.js' => '',
    'misc/farbtastic/farbtastic.js' => '',
    'misc/form.js' => '',
    'misc/jquery.ba-bbq.js' => 'http://benalman.com/projects/jquery-bbq-plugin/',
    'misc/jquery.cookie.js' => 'https://github.com/carhartl/jquery-cookie',
    'misc/jquery.form.js' => 'http://malsup.com/jquery/form/',
    'misc/jquery.js' => 'http://code.jquery.com/jquery-1.4.4.js',
    'misc/jquery.once.js' => '',
    'misc/machine-name.js' => '',
    'misc/progress.js' => '',
    'misc/states.js' => '',
    'misc/tabledrag.js' => '',
    'misc/tableheader.js' => '',
    'misc/tableselect.js' => '',
    'misc/textarea.js' => '',
    'misc/timezone.js' => '',
    'misc/ui/jquery.effects.blind.min.js' => '',
    'misc/ui/jquery.effects.bounce.min.js' => '',
    'misc/ui/jquery.effects.clip.min.js' => '',
    'misc/ui/jquery.effects.core.min.js' => '',
    'misc/ui/jquery.effects.drop.min.js' => '',
    'misc/ui/jquery.effects.explode.min.js' => '',
    'misc/ui/jquery.effects.fade.min.js' => '',
    'misc/ui/jquery.effects.fold.min.js' => '',
    'misc/ui/jquery.effects.highlight.min.js' => '',
    'misc/ui/jquery.effects.pulsate.min.js' => '',
    'misc/ui/jquery.effects.scale.min.js' => '',
    'misc/ui/jquery.effects.shake.min.js' => '',
    'misc/ui/jquery.effects.slide.min.js' => '',
    'misc/ui/jquery.effects.transfer.min.js' => '',
    'misc/ui/jquery.ui.accordion.min.js' => '',
    'misc/ui/jquery.ui.autocomplete.min.js' => '',
    'misc/ui/jquery.ui.button.min.js' => '',
    'misc/ui/jquery.ui.core.min.js' => '',
    'misc/ui/jquery.ui.datepicker.min.js' => '',
    'misc/ui/jquery.ui.dialog.min.js' => '',
    'misc/ui/jquery.ui.draggable.min.js' => '',
    'misc/ui/jquery.ui.droppable.min.js' => '',
    'misc/ui/jquery.ui.mouse.min.js' => '',
    'misc/ui/jquery.ui.position.min.js' => '',
    'misc/ui/jquery.ui.progressbar.min.js' => '',
    'misc/ui/jquery.ui.resizable.min.js' => '',
    'misc/ui/jquery.ui.selectable.min.js' => '',
    'misc/ui/jquery.ui.slider.min.js' => '',
    'misc/ui/jquery.ui.sortable.min.js' => '',
    'misc/ui/jquery.ui.tabs.min.js' => '',
    'misc/ui/jquery.ui.widget.min.js' => '',
    'misc/vertical-tabs.js' => '',
    'modules/block/block.js' => '',
    'modules/book/book.js' => '',
    'modules/color/color.js' => '',
    'modules/color/preview.js' => '',
    'modules/comment/comment-node-form.js' => '',
    'modules/contextual/contextual.js' => '',
    'modules/dashboard/dashboard.js' => '',
    'modules/field/modules/text/text.js' => '',
    'modules/field_ui/field_ui.js' => '',
    'modules/file/file.js' => '',
    'modules/filter/filter.admin.js' => '',
    'modules/filter/filter.js' => '',
    'modules/locale/locale.datepicker.js' => '',
    'modules/locale/tests/locale_test.js' => '',
    'modules/menu/menu.admin.js' => '',
    'modules/menu/menu.js' => '',
    'modules/node/content_types.js' => '',
    'modules/node/node.js' => '',
    'modules/openid/openid.js' => '',
    'modules/overlay/overlay-child.js' => '',
    'modules/overlay/overlay-parent.js' => '',
    'modules/path/path.js' => '',
    'modules/profile/profile.js' => '',
    'modules/shortcut/shortcut.admin.js' => '',
    'modules/simpletest/simpletest.js' => '',
    'modules/system/system.cron.js' => '',
    'modules/system/system.js' => '',
    'modules/taxonomy/taxonomy.js' => '',
    'modules/toolbar/toolbar.js' => '',
    'modules/user/user.js' => '',
    'modules/user/user.permissions.js' => '',
    'themes/bartik/color/preview.js' => '',
  );
}

function librejs_admin_settings($form, &$form_state) {
  $core_js = librejs_core_js();
  $licenses = librejs_licenses();
  $gpl_only = array('http://www.gnu.org/licenses/gpl-2.0.html' => 'GNU-GPL-2.0-or-later');
  $form['explanation']['#markup'] = 'Select "GNU-GPL-2.0-or-later" for Javascript included with modules downloaded from Drupal.org. Select "Expat" for MIT-licensed JavaScript (Expat license is a synonyn for MIT license).';
  $form['table'] = array(
    '#type'     => 'tableform',
    '#tf_header'   => array(
      t('Script URL'),
      t('License'),
      t('Source URL'),
    ),
  );
  $result = db_query('SELECT id, data, license, source FROM {librejs} ORDER BY data');
  foreach ($result as $row) {
    $js = array($row->data, $row->license, $row->source);
    $core = isset($core_js[$row->data]);
    $source = !empty($core_js[$row->data]);
    $item = array(
      l($row->data, file_create_url($row->data)),
      'license-' . $row->id => array(
        '#type'          => 'select',
        '#options'       => $core ? $gpl_only : $licenses,
        '#default_value' => $core ? 'http://www.gnu.org/licenses/gpl-2.0.html' : ($row->license ? $row->license : ''),
      ),
      'source-' . $row->id => array(
        '#type'          => 'textfield',
        '#size'          => 40,
        '#default_value' => $source ? $core_js[$row->data] : ($row->source ? $row->source : ''),
        '#disabled' => $core,
      ),
    );
    $form['table']['#tf_rows'][] = $item;
  }
  $form['submit'] = array(
    '#type'     => 'submit',
    '#value'    => t('Submit'),
  );
  return $form;
}

function librejs_admin_settings_submit($form, &$form_state) {
  foreach ($form_state['values'] as $field => $value) {
    if (preg_match('/source-(.*)/', $field, $matches)) {
      db_update('librejs')
        ->fields(array('source' => $form_state['values']['source-' . $matches[1]], 'license' => $form_state['values']['license-' . $matches[1]]))
        ->condition('id', $matches[1])
        ->execute();
    }
  }
}
