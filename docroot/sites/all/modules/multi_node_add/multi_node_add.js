/**
 * @file multi_node_add.js
 *   Handles IFrame generation, node mass submission and prepopulation
 */

(function ($) {

  function multiNodeAddFields() {
    var fields = [];
    $("input.multi-node-add").each(function () {
      var checker = (typeof ($(this).prop) == 'function') ? 'prop' : 'attr';
      if ($(this)[checker]('checked')) {
        fields.push($(this).attr('value'));
      }
    });
    return fields.join(',');
  }

  function multiNodeAddCheckConfig(numForms) {
    if (!('multiNodeAddPreload' in Drupal.settings)) {
      if ($("input.multi-node-add:checked").size() == 0) {
        throw Drupal.t('Fields to manage: Select at least one field');
      }
    }
    var numTest = /^[0-9]+$/;
    if (!numTest.test(numForms)) {
      throw Drupal.t('Number of rows: Specify a number greater than zero');
    }
    // @todo: what to use instead of hard-wired number? A value based on user agent? 
    if (numForms > 500) {
      if (!confirm(Drupal.t('Creating many forms at one round can freeze the browser. Do you want to continue?'))) {
        return false;
      }
    }
    return true;
  }
  var multiNodeAddNumNodes = 0;

  function multiNodeAddShowForms(numForms, view) {
    if (!multiNodeAddCheckConfig(numForms)) {
      return false;
    }
    for (var i = 0; i < numForms; i++) {
      var fields = '';
      if (Drupal.settings.multiNodeAddPreload) {
        fields = Drupal.settings.multiNodeAddPreload.fields;
      }
      else {
        fields = multiNodeAddFields();
      }
      $("#multi_node_add_frames").append('<div><iframe class="autoHeight" width="100%" name="node_' + (multiNodeAddNumNodes++) + '" src="' + Drupal.settings.multiNodeAdd.callback + '/' + fields + '?view=' + view + '"></iframe></div>');
    }

    // Takes care of the height of the iFrames
    $('#multi_node_add_frames').find('iframe.autoHeight').one('load', function() {
      var doc = $(this)[0].contentWindow.document;
      $(this).attr(
        'height', ((doc.documentElement.scrollHeight || doc.body.scrollHeight) + 10) + "px"
      );
    });

    return true;
  }

  Drupal.behaviors.multiNodeAdd = {
    attach: function (context) {

      if ('multiNodeAddPreload' in Drupal.settings) {
        try {
          multiNodeAddShowForms(Drupal.settings.multiNodeAddPreload.num, Drupal.settings.multiNodeAddPreload.view);
        } catch (err) {
          alert(err);
        }
      }
      else {

        $('#multi-node-add-page .second-step', context).hide(0);

        $('#edit-show', context).click(function () {
          try {
            if (multiNodeAddShowForms($("#edit-number").val(), $("#edit-view").attr('checked') === false ? 0 : 1)) {
              $(this).hide(0);
              $('#edit-shortcut').hide(0);
              $('#multi-node-add-page .second-step', context).show(0);
              $('input.form-checkbox.multi-node-add').attr('disabled', 'disabled');
            }
          }
          catch (err) {
            alert(err);
          }
          return false;
        });

        $('#edit-shortcut', context).click(function () {
          try {
            var numForms = $("#edit-number").val();
            var view = $("#edit-view").attr('checked') === false ? 0 : 1;
            if (!multiNodeAddCheckConfig(numForms)) {
              return false;
            }
            alert(window.location + "?fields=" + multiNodeAddFields() + "&num=" + numForms + "&view=" + view);
          } catch (err) {
            alert(err);
          }
          return false;
        });
      }

      $('#edit-addmore', context).click(function () {
        try {
          multiNodeAddShowForms(2);
        } catch (err) {
          alert(err);
        }
        return false;
      });

      $('#edit-create', context).click(function () {
        for (var i = 0; i < multiNodeAddNumNodes; i++) {
          try {
            window.frames['node_' + i].document.forms[0].submit();
          } catch (err) {} // not an error, maybe submitted w/ the single Create button
        }
        return false;
      });

      $('#edit-prepopulate', context).click(function() {

        if (multiNodeAddNumNodes < 2 || $('input[type=submit]', frames['node_0'].document).length == 0) {
          alert(Drupal.t('There must be at least two nodes and the first one must not be submitted already'));
          return false;
        }

        $('input:not([type=hidden]):not([type=submit]):not([type=button]):visible,' +
          'select:visible,' +
          'textarea:visible', frames['node_0'].document).each(function() {

            for (var i = 1; i < multiNodeAddNumNodes; i++) {
              try {
                if (($(this).is(':radio, :checkbox')) && $(this).is(':checked')) {
                  $('#' + $(this).attr('id'), frames['node_' + i].document).attr('checked', true);
                } else {
                  $('#' + $(this).attr('id'), frames['node_' + i].document).val($(this).val());
                }
              } catch (err) {} // not an error, maybe submitted w/ the single Create button
            }


        });
        return false;
      });


    }
  };

})(jQuery);
