(function ($) {

  /**
   * Custom behavior for the "Save" button.
   */
  Drupal.behaviors.viewsSave = {
    attach: function (context, settings) {
      $('form.views-save-form', context).once(function () {
        var $form = $(this);

        var $save_button = $form.find('.views-save-button-save');
        if (!$save_button.length) {
          return;
        }
        $form.after($save_button);
        $save_button.removeClass('element-invisible');
        $save_button.click(views_save_show_popup);

        $save_button.data('form', $form.attr('id'));
        $form.data('save_button', $save_button.attr('id'));

        $form.hide();

        $('a.views-save-cancel', $form).click(function() {Drupal.CTools.Modal.dismiss(); return false;});

        var id = $('.views-save-button-submit', $form).attr('id');
        if (id) {
          Drupal.ajax[id].success = views_save_success;
          Drupal.ajax[id].dataType = 'xml';
        }
      });

      /**
       * Shows the form as a popup.
       */
      function views_save_show_popup() {
        var $this = $(this);
        if (!$this.data('form')) {
          return false;
        }
        views_save_clean_settings(settings.views_save);
        var $form = $this.parent().find('#' + $this.data('form'));
        Drupal.CTools.Modal.show(settings.views_save.modal_settings);
        $('#modal-title').html(settings.views_save.modal_title);
        $('#modal-content').empty().append($form);
        $form.show();

        $(document).bind('CToolsDetachBehaviors.views_save', views_save_hide_popup);

        return false;
      }

      /**
       * Cleans the given settings objects from duplicate values.
       *
       * When receiving new settings via an AJAX request, Drupal merges those
       * with the existing settings. In our case, this leads to every scalar
       * setting becoming an array with two (or, with consecutive requests,
       * more) values, which prevents any of the "Save" button functionality to
       * work.
       *
       * Therefore, we here traverse the given settings object and, if we
       * encounter array values, use their latest value instead.
       *
       * @param object
       *   The settings object to check for duplicates.
       */
      function views_save_clean_settings(object) {
        for (var i in object) {
          if (object.hasOwnProperty(i) && typeof object[i] == 'object') {
            if (object[i] instanceof Array) {
              object[i] = object[i][object[i].length - 1]
            }
            else {
              views_save_clean_settings(object[i]);
            }
          }
        }
      }

      /**
       * Hides the popup.
       */
      function views_save_hide_popup() {
        var $form = $(this).find('form.views-save-form');
        var $save_button = $('#' + $form.data('save_button'));
        // If we don't move the form before closing the modal, its set DOM
        // handlers will be removed.
        $form.hide();
        $form.insertAfter($save_button);
        $(document).unbind('CToolsDetachBehaviors.views_save');
      }

      /**
       * Hides the popup and replaces the "Save" button with its content.
       */
      function views_save_success(response) {
        var content = '';
        for (var i in response) {
          if (response.hasOwnProperty(i) && response[i].command == 'insert' && response[i].data) {
            content += response[i].data;
          }
        }
        var $content = $(content);
        var $form = $(this.element).closest('form');
        var $save_button = $('#' + $form.data('save_button'));
        $save_button.hide().after($content);
        Drupal.attachBehaviors($content, settings);
        Drupal.CTools.Modal.dismiss();
      }
    }

  };

  /**
   * Provide the HTML to create the modal dialog.
   */
  Drupal.theme.prototype.ViewsSavePopup = function () {
    return Drupal.settings.views_save.modal_theme;
  };

})(jQuery);
