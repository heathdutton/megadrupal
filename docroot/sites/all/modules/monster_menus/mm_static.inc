<?php

/**
 * @file
 * A collection of large, static strings--such as Javascript and HTML code--in MM
 */

/**
 * Return (or add to the current page) some Javascript/HTML/text used in MM
 *
 * @param $which
 *   Key of the code to return/add
 * @param $add_js
 *   If TRUE, add the code to the current page with drupal_add_js(). If
 *   'return_once', only return the code if it is not already present.
 * @param $additional
 *   Any additional parameters are passed to the underlying function
 * @return
 *   The Javascript code
 */
function mm_static($which, $add_js = FALSE) {
  $added = &drupal_static(__FUNCTION__, array());

  $args = func_get_args();
  array_splice($args, 0, 2);
  // While this could be done with an array, that would be less efficient.
  // Instead, require that a function of the format "_mm_static__$which" exists
  // and returns some text.
  $name = "_mm_static__$which";
  if (!function_exists($name)) return;
  $code = call_user_func_array($name, $args);
  if (!empty($code)) {
    // If the code uses jQuery, add a prefix and suffix to initialize "$".
    if (strpos($code, '$(') !== FALSE) {
      $code = "(function (\$) {\n{$code}})(jQuery);";
    }

    if ($add_js && empty($added[$which])) {
      if ($add_js === TRUE) {
        drupal_add_js($code, array('type' => 'inline', 'scope' => 'footer'));
      }
      $added[$which] = TRUE;
    }
    else if ($add_js === 'return_once') {
      return '';
    }
  }
  return $code;
}

/* Referenced by mm_ui_content_edit */
function _mm_static__edit_cat() {
  return <<<JS
Drupal.behaviors.MMEditCat = {
  attach: function (context, settings) {
    \$('.settings-archive', context).once('settings-archive', function () {
      var outerDiv = this;
      if (\$('.form-item .form-checkbox', \$(this).parent())
        .click(function () {
          \$(outerDiv).slideToggle('fast');
        }).is(':checked')) {
          \$(this).show();  // init
      }
    });
    \$('.settings-node-types:checkbox', context).once('settings-node-types-checkbox', function () {
      var outerDiv = $(this).parents('fieldset')[0];
      if (\$(this)
        .click(function () {
          \$('select', outerDiv).attr('disabled', this.checked);
        }).is(':checked')) {
          \$('select', outerDiv).attr('disabled', true);  // init
      }
    });
  }
};
JS
  . _mm_static__settings_perms();
}

/* Referenced by mm_ui_content_edit, _mm_ui_node_form_perms */
function _mm_static__settings_perms($form_id = 'mm-ui-content-edit', $submit_id = 'edit-submit', $row_selector = MM_PERMS_READ) {
  $_mmlist_instance = &drupal_static('_mmlist_instance', 0);

  $elts = monster_menus_element_info();
  $root = $elts['mm_grouplist']['#mm_list_browser'];
  $popup_URL = mm_content_groups_mmtid();
  $popup_base = base_path() . "$root/$popup_URL-" . MM_BROWSER_MODE_GROUP . "-$_mmlist_instance-" . $elts['mm_grouplist']['#mm_list_enabled'] . '-' . $elts['mm_grouplist']['#mm_list_selectable'] . '/';
  $add_groups_label = t('Add a group to the permissions');
  $_mmlist_instance++;
  $r = MM_PERMS_READ;
  $w = MM_PERMS_WRITE;
  $disabled_selector = $row_selector == MM_PERMS_READ ? MM_PERMS_WRITE : $row_selector;

  return <<<JS
Drupal.behaviors.MMSettingsPerms = {
  attach: function (context) {
    \$('.mm-permissions', context).once('mm-permissions', function () {
      \$(':checkbox:not(#edit-node-everyone)', this)
        .change(function (event, recurs) {
          var id = this.name.match(/(?:^|\[)(\w+)-(\w)-(\w+)/);    // user-mode-uid or group-mode-gid
          var row = \$(this).closest('tr');
          var tbody = \$(this).closest('tbody');
          var list;
          \$('form#$form_id .messages').remove();
          if (id[3] == 'everyone') {
            // Everything else in column
            list = \$(':checkbox:not(.mm-permissions-disabled)[name*=-' + id[2] + ']:not([name="' + this.name + '"])', tbody);
            if (this.checked) list.attr('checked', true);
            list
              .toggleClass('mm-permissions-col-disabled', this.checked)
              .attr('disabled', this.checked);
            if (!recurs) {
              \$(':checkbox:checked:not([name="' + this.name + '"])', tbody)
                .trigger('change', true);
            }
          }

          if (id[2] != '$r') {
            if (id[2] == '$w') {
              // The rest of the row
              list = \$(':checkbox:not([name="' + this.name + '"],.mm-permissions-disabled,.mm-permissions-col-disabled)', row);
            }
            else {
              // r
              list = \$(':checkbox[name*=-$r-]:not(.mm-permissions-disabled,.mm-permissions-col-disabled)', row);
            }

            list.attr('disabled', this.checked);
            if (this.checked) list.attr('checked', true);
            else if (!recurs) {
              \$(':checkbox:checked[name^=' + id[1] + '-]:not([name="' + this.name + '"],[name*=-$r-])', row)
                .trigger('change', true);
            }
          }
        });

      // node perms "everyone"
      \$('#edit-node-everyone', this)
        .change(function () {
          if (\$(this).is(':checked')) {
            \$(this)
              .closest('tbody')
              .children('tr.mm-permissions-data-row')
              .fadeOut(500, function () {
                \$(this).remove();
              });
          }
        });

      // initial setup
      \$('.mm-permissions-group-new,.mm-permissions-user-new', this)
        .closest('tr')
        .hide()
        .next()
        .andSelf()
        .addClass('mm-permissions-new-row');
      \$('.mm-permissions-group-new', this)
        .closest('tr')
        .addClass('mm-permissions-group-new');
      \$('.mm-permissions-user-new', this)
        .closest('tr')
        .addClass('mm-permissions-user-new');
      var form = \$(this).closest('form');

      // on submit, process collected data into single fields
      form.once('mm-permissions', function() {
        form.submit(function () {
          if (form.find('#edit-no-save').val() * 1) {
            \$('.mm-permissions-all-values-user', this).val('');
            \$('.mm-permissions-all-values-group', this).val('');
            return;
          }
          var removed = {user: [], group: []};
          \$('.mm-permissions', form).each(function() {
            var data = {user: '', group: ''}, used = {user: [], group: []};
            \$(this)
              .find('.mm-permissions-data-row :checkbox:checked:enabled,.mm-permissions-data-row :input[type=hidden]')
              .each(function () {
                var id = this.name.match(/(?:^|\[)(\w+)-(\w)-(\w+)/);
                if (id && id[3] != 'everyone') {
                  data[id[1]] += id[2] + id[3];
                  used[id[1]][id[3]] = true;
                }
              });
            var parent = \$(this).parent();
            \$('.mm-permissions-all-values-user', parent).val(data.user);
            \$('.mm-permissions-all-values-group', parent).val(data.group);
            data = null;
            \$(this)
              .find('.mm-permissions-data-row :checkbox[name*=-$disabled_selector-]:not(.mm-permissions-disabled:checked)')
              .each(function () {
                var id = this.name.match(/(?:^|\[)(\w+)-\w-(\w+)/);
                if (id && id[2] != 'everyone' && !used[id[1]][id[2]]) {
                  removed[id[1]].push(this);
                }
              });
          });
          var usersLen = removed.user.length;
          var all = \$.merge(removed.user, removed.group);
          if (all.length) {
            var container = \$('.mm-permissions:first').closest('fieldset');
            if (!container.length) container = \$('.mm-permissions:first').closest('div');
            container.closest('fieldset.collapsed').find('a:first').click();
            var conf;
            if (usersLen && removed.group.length) {
              conf = Drupal.t('Note: @indiv individual(s) and @grps group(s) will be removed, because "All Users" already have these same permissions.', {'@indiv' : usersLen, '@grps' : removed.group.length});
            }
            else if (usersLen) {
              conf = Drupal.t('Note: @indiv individual(s) will be removed, because "All Users" already have these same permissions.', {'@indiv' : usersLen});
            }
            else {
              conf = Drupal.t('Note: @grps group(s) will be removed, because "All Users" already have these same permissions.', {'@grps' : removed.group.length});
            }
            all = \$(all).closest('tr');
            \$('form#$form_id .messages').remove();
            var msg = \$('<div class="messages status"><h2>' + conf + '</h2><input type="button" value="' + Drupal.t('Continue Anyway') + '"></div>').prependTo(container);
            \$(window).scrollTop(container.offset().top);
            \$(':button', msg).click(function() {
              all.remove();
              form.find('#$submit_id').click();
            });
            var oldColor = all.css('background-color'), done;
            all.css('background-color', '#fcc').fadeTo(200, 0.25).fadeTo(200, 1).fadeTo(200, 0.25,
              function() {
                if (!done) {
                  all.css('background-color', oldColor);
                  done = true;
                }
              }).fadeTo(200, 1);
            return false;
          }
        });
      });
      // fire all the checkbox change events, to handle disabling
      \$(':checkbox:checked', this).each(function () {
        \$(this).trigger('change');
      });
    });
  }
};

Drupal.MMSettingsUpdateSummary = function (obj) {
  \$(obj).closest('fieldset.vertical-tabs-pane').trigger('summaryUpdated');  // update any fieldset summary
};

Drupal.MMSettingsPermsDelete = function (obj) {
  \$(obj).closest('tr').fadeOut(500, function () {
    Drupal.MMSettingsUpdateSummary(this);
    \$(this).remove();
  });
  \$('form#$form_id .messages').remove();
  return false;
};

Drupal.MMSettingsPermsAddUsers = function (mmListObj, element, instance) {
  if (mmListObj.length) {
    var context = \$('[id=mm-add-users-' + instance + ']').closest('.mm-permissions');
    \$('form#$form_id .messages').remove();
    tb_remove();
    var i = 1, matches = mmListObj.val().split(/(.*?)\{(.*?)\}/);
    var hidden_row = \$('tr.mm-permissions-user-new:hidden', context);
    var obj = \$('.mm-permissions [id^=edit-user-$row_selector-]:checkbox:visible:not([id=edit-user-$row_selector-owner]):last', context).closest('tr');
    // If no previous row or limit_write is set, use default row as source of copy
    if (!obj.length || \$('[name=limit_write_not_admin]').length) obj = hidden_row;
    var new_row = [], dups = [];
    for (; i < matches.length; i += 3) {
      var dup = \$('.mm-permissions-data-row :checkbox[name=user-$row_selector-' + matches[i] + '],.mm-permissions-data-row :input[type=hidden][name=user-w-' + matches[i] + ']', context);
      if (dup.length) {
        \$.merge(dups, dup);
      }
      else {
        \$.merge(new_row,
          \$(obj)
            .clone(true)
            .addClass('mm-permissions-new-row')
            .addClass('mm-permissions-data-row')
            .find('td:eq(0) div') // set user's name
              .html(matches[i + 1])
              .end()
            .find(':checkbox,:input[type=hidden]')  // rename checkboxes using uid
              .each(function () {
                $(this).attr('name', this.name.replace(/(\w+-\w-)\w+(?=\]|\$)/, '\$1' + matches[i]));
              })
              .end()
            .fadeTo(0, 0)
        );
      }
    }
    mmListObj[0].delAll();
    \$(mmListObj[0].mmList.p.autoCompleteObj).val('');
    Drupal.MMSettingsAnimateDups(dups);
    if (new_row.length) {
      // wait for thickbox to close
      setTimeout(function () {
        \$('#edit-node-everyone', context).attr('checked', false);
        \$(new_row)
          .insertBefore(hidden_row)
          .show()
          .fadeTo(500, 1);  // fadeIn doesn't work correctly with TRs
        if (obj == hidden_row) {
          // fire all the checkbox change events, to handle disabling
          \$(':checkbox:checked', hidden_row.closest('table')).each(function () {
            \$(this).trigger('change');
          });
        }
        Drupal.MMSettingsUpdateSummary(hidden_row);
      }, 500);
    }
  }
  else {
    tb_remove();
  }
  return false;
};

Drupal.MMSettingsPermsAddGroup = function (activator) {
  var url = '$popup_base' + '$popup_URL' + '?TB_iframe=true&height=' + (window.innerHeight - 100) + '&width=' + (window.innerWidth - 100);
  window.mmListInstance = {
    addFromChild: function (chosen, info) {
      var context = \$(activator).closest('.mm-permissions');
      var mmtid = chosen.id.substr(5);
      var hidden_row = \$('tr.mm-permissions-group-new:hidden', context);
      var obj = \$('.mm-permissions [id^=edit-group-$row_selector-]:checkbox:visible:not([id=edit-group-$row_selector-everyone]):last', context).closest('tr');
      // If no previous row or limit_write is set, use default row as source of copy
      if (!obj.length || \$('[name=limit_write_not_admin]').length) obj = hidden_row;
      var dups = \$('.mm-permissions-data-row :checkbox[name=group-$row_selector-' + mmtid + '],.mm-permissions-data-row :input[type=hidden][name=group-w-' + mmtid + ']', context);
      if (dups.length) {
        tb_remove();
        Drupal.MMSettingsAnimateDups(dups);
      }
      else {
        var new_row = \$(obj)
          .clone(true)
          .addClass('mm-permissions-new-row')
          .addClass('mm-permissions-data-row')
          .find('td:eq(0)')
            .find('fieldset') // make sure fieldset is collapsed
              .addClass('collapsed')
              .find('.fieldset-wrapper')
                .hide()
                .end()
              .end()
            .find('legend .fieldset-legend a') // copy group name
              .text(\$('a:first', chosen).html().replace(/<(\w+).*?>.*?<\/\\1>/g, '').replace(/^\s*(.*?)\s*\$/, '$1'))   // IE doesn't support trim()
              .end()
            .find('legend + div div') // copy user list
              .html(info)
              .end()
            .end()
          .find(':checkbox,:input[type=hidden]')  // rename checkboxes using mmtid
            .each(function () {
              \$(this).attr('name', this.name.replace(/(\w+-\w-)\w+(?=\]|\$)/, '$1' + mmtid));
            })
            .end()
          .fadeTo(0, 0);
        \$('form#$form_id .messages').remove();
        \$('fieldset.collapsible', new_row)
          .removeOnce('collapse')
          .find('.fieldset-title')
            .unbind('click');
        Drupal.behaviors.collapse.attach(new_row, Drupal.settings);
        tb_remove();
        // wait for thickbox to close
        setTimeout(function () {
          \$('#edit-node-everyone', context).attr('checked', false);
          \$('a.thickbox', new_row)
            .click(function () {
              tb_show(this.title || this.name || null, this.href || this.alt, this.rel || false);
              this.blur();
              return false;
            });
          new_row
            .insertBefore(hidden_row)
            .show()
            .fadeTo(500, 1);
          Drupal.MMSettingsUpdateSummary(hidden_row);
        }, 500);
      }
    }
  };
  tb_show('$add_groups_label', url, false);
  return false;
};

Drupal.MMSettingsAnimateDups = function (dups) {
  dups = \$(dups).closest('tr');
  if (dups.length) {
    // wait for thickbox to close
    setTimeout(function () {
      var old = dups.css('background-color');
      dups.css('background-color', '#ff5').fadeTo(200, 0.25).fadeTo(200, 1).fadeTo(200, 0.25).fadeTo(200, 1).fadeTo(200, 0.25).fadeTo(200, 1, function () {\$(this).css('background-color', old)});
    }, 500);
  }
};

Drupal.MMSettingsPermsOwner = function (mmListObj) {
  if (mmListObj.length) {
    var matches = mmListObj.val().split(/(.*?)\{(.*?)\}/);
    if (matches.length > 1) {
      \$('.mm-permissions .settings-perms-owner-name').text(matches[2]);
      \$('[name=owner]').val(matches[1]);
    }
  }
  tb_remove();
  return false;
};
JS;
}

/* Referenced by mm_node_redir_form */
function _mm_static__node_redir() {
  return <<<JS
Drupal.behaviors.MMNodeRedir = {
  attach: function (context, settings) {
    \$('.node-redir-url', context).once('node-redir-url', function () {
      \$(this).change(function () {
        if (this.value != '' && this.form['redir_mmtid'].value != '')
          this.form['redir_mmtid'].delAll();
      });
    });
  }
};
JS;
}

/* Referenced by mm_node_alert_add_to_footer */
function _mm_static__node_alert() {
  return <<<JS
Drupal.behaviors.MMNodeAlert = {
  attach: function (context, settings) {
    // Intentionally don't use 'once' here
    \$('.mm-alert:not(.mm-alert-processed):first', context)
      .addClass('mm-alert-processed')
      .each(function () {
        \$('.mm-alert-close :button', this).click(function () {
          if (\$('.mm-alert:not(.mm-alert-processed)').length) {
            \$('#TB_window').fadeOut('fast', function () {
              \$("#TB_ajaxContent").empty();
              Drupal.behaviors.MMNodeAlert.attach(context); // show next alert while reappearing
            }).fadeIn('fast');
          }
          else {
            tb_remove();
          }
          return false;
        });
        tb_show(null, "#TB_inline?height=250&width=300&inlineId=" + this.id + "&modal=true", false);
      });
  }
};
JS;
}

/* Referenced by mm_ui_content_copymove */
function _mm_static__copymove($mustcheck) {
  return <<<JS
Drupal.MMHideNameDiv = function () {
  if (\$('input[name=mode]')[0].checked) {
    \$('#namediv').toggle(\$('#edit-copy-page')[0].checked);
  }
  else {
    var x = \$('input[name=move_mode]');
    \$('#namediv').toggle(x.length == 2 && x[0].checked);
  }
  return true;
};

Drupal.MMHideCopyDesc = function () {
  !\$('#edit-copy-page')[0].checked && \$('#edit-copy-nodes')[0].checked ? \$('#copydiv .description').show() : \$('#copydiv .description').hide();
};

Drupal.behaviors.MMCopyMove = {
  attach: function (context) {
    \$('form.mm-ui-copymove', context).once('mm-ui-copymove', function () {
      \$('input[name=mode]', this)
        .click(function () {
          \$('#copydiv').toggle(this.value != 'move');
          \$('#movediv').toggle(this.value == 'move');
          return Drupal.MMHideNameDiv();
        });
      \$('input[name=copy_page]')
        .click(function () {
          if (!this.checked && (!\$('#edit-copy-nodes').length || !\$('#edit-copy-nodes')[0].checked)) {
            {$mustcheck}
            return false;
          }
          var x = \$('#edit-copy-subpage').attr('disabled', !this.checked).parent();
          this.checked ? x.removeClass('disabled') : x.addClass('disabled');
          Drupal.MMHideCopyDesc();
          return Drupal.MMHideNameDiv();
        });
      \$('input[name=copy_nodes]')
        .click(function () {
          if (!this.checked && !\$('#edit-copy-page').attr('checked')) {
            {$mustcheck}
            return false;
          }
        });
      \$('input[name=move_mode]')
        .click(Drupal.MMHideNameDiv);
      Drupal.MMHideCopyDesc();
      \$('input[name=mode]:checked')
        .click();
    });
  }
};
JS;
}

/* Referenced by theme_mm_archive */
function _mm_static__select_menu() {
  return <<<JS
Drupal.selectMenuAddSelect = function (ul, div, preselect) {
  var select = \$('<select />')
    .change(function (event, preselect) {
      \$(this).nextAll().remove();
      var href = this.value;  // \$(this).val() doesn't work in IE
      if (href && href[0] != '~')
        if (window.parent) window.parent.location = href;
        else document.location = href;
      else {
        \$('.select-menu-no-val', this).remove();
        \$(\$(':selected', this)[0].li).next('ul')
          .each(function () {
              Drupal.selectMenuAddSelect(this, div, preselect);
          });
      }
    })
    .attr('id', $(ul).attr('id'))
    .appendTo(div);

  var label = $('<label />')
    .attr('for', $(ul).attr('id'))
    .text($(ul).attr('title') + ':')
    .insertBefore(select)
    .after('&nbsp;');

  if (!\$('.select-menu-no-val', select).length)
    \$('<option class="select-menu-no-val" value="">' + Drupal.t('(choose)') + '<' + '/option>')
      .appendTo(select);

  var kids = 0, kid_val;
  \$(ul)
    .children('li')
      .each(function () {
        kids++;
        var a = \$('a', this);
        kid_val = a.length ? a[0].href : '~' + \$(this).text();
        \$('<option>' + \$(this).text() + '<' + '/option>')
          .appendTo(select)
          .val(kid_val)
          [0].li = this;
      });

  if (kids == 1 && preselect && !kid_val) {
    \$('.select-menu-no-val', select).remove();
    if (select.val() == '') select.trigger('change', [true]);
  }
};

Drupal.behaviors.selectMenuInit = {
  attach: function (context) {
    \$('ul.select-menu', context).once('selectMenuInit', function () {
      \$(this).hide();
      var div = \$('<div class="select-menu"/>').insertAfter(this);
      // pre-select active path
      var list = \$('a.active', this)
        .parents('ul');
      Drupal.selectMenuAddSelect(this, div, list.length == 0);
      // traverse the list backwards
      if (list.length) {
        var select = \$('select', div);
        for (var i = list.length - 1; --i >= 0 && select.length;) {
          select.val('~' + \$(list[i]).prev().text());
          select.trigger('change');
          select = select.next();
        }

        if (select.length) {
          select.val(\$('a.active', this)[0].href);
          \$('.select-menu-no-val', select).remove();
        }
      }
    });
  }
};
JS;
}

/* Referenced by theme_mm_tabbed_ui */
function _mm_static__tabbed_ui_js($selected) {
  return <<<JS
Drupal.behaviors.MMTabbedUI = {
  attach: function (context) {
    \$('.mm-tabbed-ui', context).once('mm-tabbed-ui', function () {
        \$('.mm-tabs', this)
          .tabs({selected: $selected});
        var parent = this;
        \$('h5', this)
          .click(function () {
            \$('.mm-tabs', parent).toggle();
            return false;
          });
        \$('.mm-tabs', this).show();
      });
  }
};
JS;
}

/* Referenced by mm_ui_solver */
function _mm_static__solver_mm_list_callback($mmtid) {
  return <<<JS
Drupal.mmSolverCallback = function (mmList) {
  var mmtid;
  var val = \$(mmList.hiddenElt).val();
  \$('#mm-solver-table')
    .slideUp('fast', function () {
      if (val && (mmtid = val.match(/^(\d+)\{/)) && mmtid[0] != '') {
        \$.get(Drupal.settings.basePath + 'mm/$mmtid/settings/solver/' + mmtid[1], function (data) {
          \$('#mm-solver-table')
            .html(data)
            .slideDown('fast');
        });
      }
    });
};

jQuery('#mm-solver-link a').click(function() {
  setTimeout(function() {
    jQuery('#edit-solver-choose').focus();
  }, 500);
});
JS;
}

/* Referenced by mm_browser_bookmark_add */
function _mm_static__mm_browser_bookmark_add($name, $mmtid) {
  global $base_url;

  $base = base_path();
  $mm_path = $base . drupal_get_path('module', 'monster_menus');
  $title = t('Title:');
  $add = t('Add');
  $cancel = t('Cancel');
  $name = check_plain($name);
  $title_msg = t('Please enter a title for this bookmark.');
  return <<<HTML
<html><head>
  <link rel="stylesheet" href="$mm_path/css/mm_browser_tb.css" />
  <script type="text/javascript" src="$base_url/misc/jquery.js"></script>
  <script type="text/javascript" src="$base_url/sites/all/modules/thickbox/thickbox.js"></script>
</head>
<body id="tb-body">
  <div id="message"><p>$title_msg</p></div>
  <div id="add-bookmark-div">
    <form action="#" onsubmit="return parent.Drupal.mm_browser_add_bookmark_submit(document);">
      <table border="0" cellpadding="3" cellspacing="3" style="margin:0 auto;" >
        <tr>
          <td><label>$title</label></td>
          <td>
            <input name="linktitle" id="linktitle" type="text" size="25" maxlength="35" value="$name" tabindex="1">
            <input name="linkmmtid" type="hidden" value="$mmtid">
          </td>
        </tr>
        <tr align="right">
          <td colspan="2">
            <input type="submit" id="Add" value="$add" onclick="return parent.Drupal.mm_browser_add_bookmark_submit(document);" tabindex="2">&nbsp;
            <input type="submit" id="Cancel" value="$cancel" onclick="parent.tb_remove(); return false;" tabindex="3">
          </td>
        </tr>
      </table>
    </form>
  </div>
</body></html>
HTML;
}

/* Referenced by mm_browser_bookmark_manage */
function _mm_static__mm_browser_bookmark_manage() {
  global $base_url;

  return <<<JS
\$(function () {
  \$("#sortable").sortable({
    axis:                 'y',
    containment:          'document',
    placeholder:          'ui-state-highlight',
    forcePlaceholderSize: true,
    opacity:              0.5,
    stop:                 function (event, ui) {
      var serialStr = "";
      \$("ul#sortable li").each(function () {
        serialStr += \$(this).attr("name") + "|";
      });
      \$.post(window.parent.Drupal.mm_browser_append_params("$base_url/mm-bookmarks/sort"),
        {neworder: serialStr.substring(0, serialStr.length - 1)},
        function () {
          window.parent.Drupal.mm_browser_get_bookmarks();
        },
        "json");
    }
  });
  \$("#sortable").disableSelection();
});
JS;
}

function _mm_static__back_in_history() {
  return <<<JS
Drupal.mm_back_in_history = function (save) {
  if (save) {
    document.cookie = 'mm_last_page=' + encodeURI(document.location) + ';path=/';
  }
  else {
    var matches = document.cookie.match(/\bmm_last_page=(.*?)(;|$)/);
    if (matches && matches.length && matches[1].length) {
      var date = new Date(0);
      document.cookie = "mm_last_page=;expires=" + date.toUTCString() + ";path=/";
      document.cookie = 'goto_last=1;path=/';
      document.location = matches[1];
    }
    else {
      document.cookie = 'goto_last=1;path=/';
      window.history.back(-1);
    }
    return false;
  }
};
JS;
}

/* Referenced by _mm_ui_content_edit */
function _mm_static__ui_content_edit_js($mmtid, $token, $element_name, $is_new) {
  $clean_element = str_replace('-', '_', $element_name);
  $id_name = str_replace('_', '-', $element_name);
  $columns = _mm_ui_userlist_get_headers();

  $col_def = '';
  for ($i = 0; $i < count($columns) - 1; $i++) {
    $col_def .= 'null, ';
  }
  $col_def .= '{"bSortable": false}';

  $count_text = t('Showing _START_ to _END_ of _TOTAL_ record(s)');
  $filtered_text = t('(filtered from _MAX_ total record(s))');

  $new = $is_new ? '/new' : '';
  $link_location = "Drupal.settings.basePath + '-mm-users-json/$mmtid/$token/listusers/$element_name$new'";
  $delete_location = "Drupal.settings.basePath + '-mm-users-json/$mmtid/$token/deleteusers/' + uid + '$new'";
  $add_location = "Drupal.settings.basePath + '-mm-users-json/$mmtid/$token/addusers/' + uids.replace(/\\{.*?\\}/g, ',') + '$new'";

  return <<<JS
if (typeof(Drupal.mmgroupTable) == 'undefined') Drupal.mmgroupTable = [];

Drupal.behaviors.MMUserDataTable$clean_element = {
  attach: function (context, settings) {
    \$('#mm-user-datatable-$id_name', context).once('mm-user-datatable-$id_name', function () {
      Drupal.mmgroupTable['$clean_element'] = \$('#mm-user-datatable-$id_name').dataTable({
        aoColumns:      [$col_def],
        bProcessing:    true,
        bServerSide:    true,
        iDisplayLength: 20,
        bLengthChange:  false,
        sAjaxSource:    $link_location,
        oLanguage:      {sInfo: "$count_text", sInfoFiltered: "$filtered_text"}
      });
    });
  }
};

Drupal.mmGroupRemoveUser = function (uid, element) {
  \$.ajax({
    url:     $delete_location,
    async:   false,
    success: function (msg) {
      Drupal.mmgroupTable[element].fnDraw();
    }
  });
};

Drupal.mmGroupAddUser = function (mmListObj, element) {
  var uids;
  if (mmListObj.length && (uids = mmListObj.val())) {
    \$.ajax({
      url:     $add_location,
      async:   false,
      success: function () {
        tb_remove();
        mmListObj[0].delAll();
        \$(mmListObj[0].mmList.p.autoCompleteObj).val('');
        Drupal.mmgroupTable[element].fnDraw();
      }
    });
  }
  else {
    tb_remove();
  }
  return false;
};
JS;
}

function _mm_static__lazy_load_node($mmtid, $max_pages) {
  if ($max_pages != 0) {
    $loading_message = '<img src="' . drupal_get_path('module', 'monster_menus') . '/images/ajax-loader.gif" width="24" height="24"> ' . t('Loading additional content...');
    return <<<JS
Drupal.MMLazyLoadingActiveLoad = false;

Drupal.behaviors.MMLazyLoadNode = {
  attach: function () {
    \$('.mm-lazy-load-max-page').val(1); // Make sure refreshed pages start out at zero
    \$(window).scroll(function() {
      if (Drupal.MMLazyLoadScrollCheck(\$('.node:last')) && !Drupal.MMLazyLoadingActiveLoad) {
        Drupal.MMLazyLoadContent();
      }
    })
    .trigger('scroll');
  }
};

Drupal.MMLazyLoadScrollCheck = function (elem) {
  var docViewTop = \$(window).scrollTop();
  var docViewBottom = docViewTop + \$(window).height();

  var elemTop = elem.offset().top;
  var elemBottom = elemTop + elem.height();

  return elemBottom >= docViewTop && elemTop <= docViewBottom;
};

Drupal.MMLazyLoadContent = function () {
  var maxPage = \$('.mm-lazy-load-max-page');
  var page = parseInt(maxPage.val());
  if (page < $max_pages) {
    Drupal.MMLazyLoadingActiveLoad = true;
    maxPage.before('<div id="node-loading">$loading_message</div>');
    \$.ajax({
      cache: false,
      url: Drupal.settings.basePath + "mm/$mmtid/-2/render?page=" + page,
      success: function (data) {
        \$('#node-loading').remove();
        maxPage.before(data);
        maxPage.val(page + 1);
        Drupal.MMLazyLoadingActiveLoad = false;
        if (Drupal.MMLazyLoadScrollCheck(maxPage.siblings('.node:last')) && !Drupal.MMLazyLoadingActiveLoad) {
          Drupal.MMLazyLoadContent();
        }
      }
    });
  }
};
JS;
  }
}

/* Referenced by mm_show_group */
function _mm_static__show_group($mmtid, $col_def) {
  $link_location = url('-mm-users-json/' . $mmtid . '/listusers/mm-user-datatable-members-display');
  $count_text = t('Showing _START_ to _END_ of _TOTAL_ record(s)');
  $filtered_text = t('(filtered from _MAX_ total record(s))');
  return <<<JS
Drupal.behaviors.MMShowGroup = {
  attach: function () {
    \$('#mm-user-datatable-members-display').dataTable({
      aoColumns:      [$col_def],
      bProcessing:    true,
      bServerSide:    true,
      iDisplayLength: 20,
      bLengthChange:  false,
      sAjaxSource:    '$link_location',
      oLanguage:      {sInfo: "$count_text", sInfoFiltered: "$filtered_text"}
    });
  }
};
JS;
}

/* Referenced by mm_admin_browse */
function _mm_static__admin_browse() {
  $u = MM_PERMS_APPLY;
  return <<<JS
Drupal.MMAdminBrowse = function (start, mode, selected, title) {
  var url = Drupal.settings.basePath + 'mm-browser-load/' + start + '-' + mode + '-0--$u/' + selected + '?TB_iframe=true&height=' + (window.innerHeight - 100) + '&width=' + (window.innerWidth - 100);
  tb_show(title, url, false);
  return false;
};
JS;
}

function _mm_static__settings_general_summary() {
  // Note: The code below must use the word 'jQuery' instead of '$'.
  return <<<JS
Drupal.settings.mmSummaryFuncs['edit-settings-general'] = function (context) {
  var pageName = jQuery('input[name="name"]', context).val() || Drupal.t('none specified');
  var out = Drupal.t('name: @pageName', {'@pageName' : pageName}) + '<br />';

  var alias = jQuery('input[name="alias"]', context).val() || jQuery('.mm-alias-name', context).val();
  return out + (alias ? Drupal.t('url: @url', {'@url' : alias}) : '');
};
JS;
}

function _mm_static__flags_summary() {
  // Note: The code below must use the word 'jQuery' instead of '$'.
  return <<<JS
Drupal.settings.mmSummaryFuncs['edit-flags'] = function (context) {
  var mm_flags = [];
  jQuery('.flag-checkbox', context).each(function(index) {
    if (this.type == 'checkbox') {
      if (jQuery(this).is(':checked')) {
        mm_flags.push(Drupal.checkPlain(jQuery.trim(jQuery("label[for='" + jQuery(this).attr('id') + "']").text())));
      }
    }
    else if (this.type == 'text') {
      if (jQuery(this).val() != '') {
        mm_flags.push(Drupal.checkPlain(jQuery.trim(jQuery("label[for='" + jQuery(this).attr('id') + "']").text()) + ": " + jQuery(this).val()));
      }
    }
    else {
      // Revisit this, currently not dynamically adding free form flags to the summary.
      // Tough because it's a textarea. Further tough because Drupal's AJAX stuff here
      // isn't automatically called on textarea update.
      //  Drupal.getSelection(jQuery("textarea[name='free_flags']"));
    }
  });
  return mm_flags.length < 1 ? Drupal.t("none") : mm_flags.join(', ');
};
JS;
}

function _mm_static__menu_summary() {
  // Note: The code below must use the word 'jQuery' instead of '$'.
  return <<<JS
Drupal.settings.mmSummaryFuncs['edit-menu'] = function (context) {
  return jQuery('input[name="hide_menu"]:checked', context).length ? Drupal.t('hide in menu') : Drupal.t('show in menu');
};
JS;
}

function _mm_static__defaults_summary() {
  // Note: The code below must use the word 'jQuery' instead of '$'.
  return <<<JS
Drupal.settings.mmSummaryFuncs['edit-defaults'] = function (context) {
  var attribution = Drupal.t("attribution") + ": ",
    node_info = jQuery('select[name="node_info"]', context);
  if (node_info.length && Drupal.settings.mmNodeInfo) {
    attribution += Drupal.settings.mmNodeInfo[node_info.val()];
  }
  if (Drupal.settings.monster_menus.comment_enabled) {
    var comments = Drupal.t("comments") + ": ";
    switch (jQuery('select[name="comment"]', context).val()) {
      case '0':
        comments += Drupal.t('disabled');
        break;
      case '1':
        comments += Drupal.t('read-only');
        break;
      case '2':
        comments += Drupal.t('read/write');
        break;
      default:
        comments += Drupal.t('unknown');
        break;
    }
    return attribution + '<br />' + comments;
  }
  return attribution;
};
JS;
}

function _mm_static__appearance_summary() {
  $rss = '';
  if (variable_get('mm_enable_rss', FALSE)) {
    $rss = <<<JS
  if (jQuery('input[name="rss"]', context).attr('checked')) {
    summary += Drupal.t('enable RSS') + '<br />';
  }
JS;
  }

  // Note: The code below must use the word 'jQuery' instead of '$'.
  return <<<JS
Drupal.settings.mmSummaryFuncs['edit-appearance'] = function (context) {
  var nodesPerPage = jQuery('select[name="nodes_per_page"]', context);
  var summary = Drupal.t('theme') + ': ' + (jQuery('select[name="theme"]', context).val() || Drupal.t("use parent's theme")) + '<br />';
  if (jQuery('input[name="previews"]', context).attr('checked')) {
    summary += Drupal.t('show only summaries') + '<br />';
  }
  $rss
  var num = nodesPerPage.val();
  if (!num) num = Drupal.t('inherit');
  else if (num == 0) num = Drupal.t('all');
  else if (num == -2) num = Drupal.t('all, as needed');
  summary += Drupal.t('items per page: !num', {'!num': num});
  return summary;
};
JS;
}

function _mm_static__settings_node_types_summary() {
  // Note: The code below must use the word 'jQuery' instead of '$'.
  return <<<JS
Drupal.settings.mmSummaryFuncs['edit-settings-node-types'] = function (context) {
  if (jQuery('input[name="allowed_node_types_inherit"]:checked', context).length) {
    return Drupal.t('inherit') + '<br />';
  }
  var nodeTypes = [];
  jQuery('select[name="allowed_node_types[]"]')
    .children('option:selected')
    .each(function() {
      nodeTypes.push(Drupal.checkPlain(jQuery(this).text()));
    });
  return nodeTypes.join(', ').toLowerCase();
};
JS;
}

function _mm_static__settings_archive_summary() {
  // Note: The code below must use the word 'jQuery' instead of '$'.
  return <<<JS
Drupal.settings.mmSummaryFuncs['edit-settings-archive'] = function (context) {
  if (!jQuery('input[name="archive"]:checked', context).length) {
    return Drupal.t('none');
  }
  return Drupal.t('archive') + ': ' +
    Drupal.t(jQuery('select[name="frequency"]').children('option:selected').text()) + '<br />' +
    Drupal.formatPlural(jQuery('select[name="main_nodes"]').val(), '1 item per page', '@count items per page');
};
JS;
}

function _mm_static__mm_categories_summary() {
  // Note: The code below must use the word 'jQuery' instead of '$'.
  return <<<JS
Drupal.settings.mmSummaryFuncs['edit-mm-categories'] = function (context) {
  return Drupal.formatPlural(jQuery('div.mm-list,div.mm-list-selected', context).length, '1 page', '@count pages');
};
JS;
}

function _mm_static__mm_appearance_summary() {
  // Note: The code below must use the word 'jQuery' instead of '$'.
  return <<<JS
Drupal.settings.mmSummaryFuncs['edit-mm-appearance'] = function (context) {
  var summary = [];

  var sticky = jQuery('input[name=sticky]', context);
  if (sticky.length) {
    summary.push(sticky.attr('checked') ? Drupal.t('sticky') : Drupal.t('not sticky'));
  }

  var attribution = jQuery('select[name="show_node_info"]', context);
  if (attribution.length) {
    summary.push(Drupal.t("attribution") + ": " + Drupal.settings.mmNodeInfo[attribution.val()]);
  }

  var date = jQuery('input[name=date],input[name="date[date]"],input[name="date[time]"]', context);
  if (date.length) {
    summary.push(Drupal.t("authored") + ": " + date.eq(0).val() + (date.length > 1 ? ' ' + date.eq(1).val() : ''));
  }

  return summary.join('<br />');
};
JS;
}

function _mm_static__publishing_summary() {
  // Note: The code below must use the word 'jQuery' instead of '$'.
  return <<<JS
Drupal.settings.mmSummaryFuncs['edit-publishing'] = function (context) {
  var concat_date = function(path) {
    var out = [];
    jQuery(path, context).each(function() {
      var v;
      if (v = jQuery(this).val()) {
        out.push(v);
      }
    });
    return out.join(' ');
  };
  var summary = [];

  var check = jQuery('input[name=status]', context);
  if (check.length) {
    summary.push(check.attr('checked') ? Drupal.t('published') : Drupal.t('not published'));
  }

  var pub = concat_date('input[name=publish_on],input[name="publish_on[date]"],input[name="publish_on[time]"]');
  if (pub) {
    summary.push(Drupal.t("publish on") + ": " + pub);
  }

  var pub = concat_date('input[name=unpublish_on],input[name="unpublish_on[date]"],input[name="unpublish_on[time]"]');
  if (pub) {
    summary.push(Drupal.t("unpublish on") + ": " + pub);
  }

  check = jQuery('input[name=set_change_date]', context);
  if (check.attr('checked')) {
    summary.push(Drupal.t('use "Publish on" date'));
  }

  return summary.join('<br />');
};
JS;
}

function _mm_static__settings_perms_summary() {
  // Note: The code below must use the word 'jQuery' instead of '$'.
  return <<<JS
Drupal.settings.mmSummaryFuncs['edit-settings-perms'] = function (context) {
  var summary = [];
  var owner = Drupal.t('owner:') + ' ' + jQuery('.settings-perms-owner-name', context).text();

  var check = jQuery('input[name=node-everyone]', context);
  if (check.attr('checked')) {
    summary.push(Drupal.t('everyone'));
    summary.push(owner);
  }
  else {
    summary.push(owner);
    summary.push(Drupal.formatPlural(jQuery('.mm-permissions-data-row :hidden+.form-type-item', context).length, '1 individual', '@count individuals'));
    summary.push(Drupal.formatPlural(jQuery('.mm-permissions-data-row fieldset', context).length, '1 group', '@count groups'));
  }

  return summary.join('<br />');
};
JS;
}

function _mm_static__mm_ui_node_reorder() {
  /**
   * Move a block in the blocks table from one region to another via select list.
   *
   * This behavior is dependent on the tableDrag behavior, since it uses the
   * objects initialized in that behavior to update the row.
   */
  return <<<JS
Drupal.behaviors.nodeReorderDrag = {
  attach: function (context, settings) {
    // tableDrag is required and we should be on the blocks admin page.
    if (typeof Drupal.tableDrag == 'undefined' || typeof Drupal.tableDrag['mm-reorder1'] == 'undefined' && typeof Drupal.tableDrag['mm-reorder2'] == 'undefined') {
      return;
    }

    var table = $('table#mm-reorder1,table#mm-reorder2');
    var tableDrag1 = Drupal.tableDrag['mm-reorder1'] || {row: {prototype: {}}}; // Sticky tableDrag object.
    var tableDrag2 = Drupal.tableDrag['mm-reorder2'] || {row: {prototype: {}}}; // Unsticky tableDrag object.

    // Add a handler for when a row is swapped, update empty regions.
    tableDrag1.row.prototype.onSwap = tableDrag2.row.prototype.onSwap = function (swappedRow) {
      var rowObject = this;
      $('tr.region-message', table).each(function () {
        // If the dragged row is in this region, but above the message row, swap it down one space.
        if ($(this).prev('tr').get(0) == rowObject.element) {
          // Prevent a recursion problem when using the keyboard to move rows up.
          if ((rowObject.method != 'keyboard' || rowObject.direction == 'down')) {
            rowObject.swap('after', this);
          }
        }
        // This region has become empty.
        if ($(this).next('tr').is(':not(.draggable)') || $(this).next('tr').length == 0) {
          $(this).removeClass('region-populated').addClass('region-empty');
        }
        // This region has become populated.
        else if ($(this).is('.region-empty')) {
          $(this).removeClass('region-empty').addClass('region-populated');
        }
      });
    };

    tableDrag1.row.prototype.isValidSwap = tableDrag2.row.prototype.isValidSwap = function (row) {
      return !$(row).is('.mm-not-draggable') && !$(row).is('.region-title') && (row != this.table.tBodies[0].rows[0] || $(row).is('.draggable'));
    };

    // A custom message for the blocks page specifically.
    Drupal.theme.tableDragChangedWarning = function () {
      return '<div class="messages warning">' + Drupal.theme('tableDragChangedMarker') + ' ' + Drupal.t('The changes to this table will not be saved until the <em>Save configuration</em> button is clicked.') + '</div>';
    };

    tableDrag1.onDrag = tableDrag2.onDrag = function () {
      var dragObject = this;
      var regionField = $('select.nodes-region-select', dragObject.rowObject.element);
      $(this.rowObject.element).siblings('tr.draggable,tr.region-message').each(function() {
        var regionRow = $(this).is('.region-message') ? this : $(this).prevAll('tr.region-message').get(0);
        var regionName = regionRow.className.replace(/([^ ]+[ ]+)*region-([^ ]+)-message([ ]+[^ ]+)*/, '$2');
        if ($('option[value=' + regionName + ']', regionField).length == 0) {
          $(regionRow).prev().add(this).addClass('mm-not-draggable');
        }
      });
    };

    // Add a handler so when a row is dropped, update fields dropped into new regions.
    tableDrag1.onDrop = tableDrag2.onDrop = function () {
      $(this.rowObject.element).siblings('tr.mm-not-draggable').removeClass('mm-not-draggable');
      var dragObject = this;
      // Use "region-message" row instead of "region" row because
      // "region-{region_name}-message" is less prone to regexp match errors.
      var regionRow = $(dragObject.rowObject.element).prevAll('tr.region-message').get(0);
      var regionName = regionRow.className.replace(/([^ ]+[ ]+)*region-([^ ]+)-message([ ]+[^ ]+)*/, '$2');
      var regionField = $('select.nodes-region-select', dragObject.rowObject.element);
      // Check whether the newly picked region is available for this node.
      if ($('option[value=' + regionName + ']', regionField).length == 0) {
        // If not, alert the user and keep the node in its old region setting.
        alert(Drupal.t('The content cannot be placed in this region.'));
        // Simulate that there was a selected element change, so the row is put
        // back to from where the user tried to drag it.
        regionField.change();
      }
      else if ($(dragObject.rowObject.element).prev('tr').is('.region-message')) {
        var weightField = $('select.nodes-weight', dragObject.rowObject.element);
        var oldRegionName = weightField[0].className.replace(/([^ ]+[ ]+)*nodes-weight-([^ ]+)([ ]+[^ ]+)*/, '$2');

        if (!regionField.is('.nodes-region-' + regionName)) {
          regionField.removeClass('nodes-region-' + oldRegionName).addClass('nodes-region-' + regionName);
          weightField.removeClass('nodes-weight-' + oldRegionName).addClass('nodes-weight-' + regionName);
          regionField.val(regionName);
        }
      }
    };
  }
};
JS;
}
