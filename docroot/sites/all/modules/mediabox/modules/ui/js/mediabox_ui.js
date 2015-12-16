"use strict";

Drupal.mediabox = Drupal.mediabox || {};

(function ($) {

/**
 * Add items mediabox browser dialog callback.
 */
Drupal.mediabox.mediaboxBrowserDialogAdd = function ($dialog, settings) {
  var str = '';
  $dialog.find('.mediabox-selected').each(function (key, element) {
    str += $(element).attr('id').toString() + ",";
  });

  // Trigger hidden form interface for adding new mediabox displays.
  $('#' + settings.field + '_mid_elements').attr('value', str);
  $('#' + settings.field + '_add_more').trigger('mousedown');
};

/**
 * Plupload complete callback.
 */
Drupal.mediabox.uploadCompleteCallback = function (up, files) {
  var $this = $("#" + up.settings.container);
  // We are not using click event because Drupal ajax framework except mousedown
  // event.
  $this.closest('form').find('.form-submit').mousedown();
};

function hideMediaboxItem(context) {
  var $checkbox = $('input', context);
  var $item = $checkbox.parent().parent().next().children();
  if ($checkbox.is(':checked')) {
    $item.slideUp('fast');
  } else {
    $item.slideDown('fast');
  }
}

Drupal.behaviors.mediaboxBrowser = {

  attach:function (context, settings) {

    $('.mediabox-remove .form-type-checkbox', context).once('mediabox-remove', function () {
      var $context = $(this);
      $context.click(function () {
        var $checkbox = $('input', this);

        if ($checkbox.is(':checked')) {
          $context.removeClass('mediabox-restore');
          $checkbox.attr('checked', false);
        } else {
          $context.addClass('mediabox-restore');
          $checkbox.attr('checked', true);
        }

        hideMediaboxItem($context);
      });
    });

    $('.mediabox-remove .form-type-checkbox', context).each(function (index) {
      var $context = $(this);
      var $checkbox = $('input', this);

      if ($checkbox.is(':checked')) {
        $context.addClass('mediabox-restore');
      } else {
        $context.removeClass('mediabox-restore');
      }

      hideMediaboxItem($context);
    });
  }

};

/**
 * This is behavior and not callback because dialog content can change over
 * ajax - for example views ajax pagers.
 */
Drupal.behaviors.mediaboxBrowserDialog = {
  attach: function (context, settings) {
    $('#mediabox-ui-library', context).once('mediabox-browser-dialog', function () {
      var selected = 0;
      var fieldCardinality = parseInt(Drupal.mediabox.settings.field_cardinality);
      var ids = Drupal.mediabox.settings.ids;
      // Don't allow to be selected more than one image (take last submitted)
      if (ids) {
        if (fieldCardinality === 1) {
          ids = ids.slice(-1);
        }
        for (var i = 0; i < ids.length; i++) {
          $('#' + ids[i], context).toggleClass('mediabox-selected');
          ++selected;
        }
      }
      $('.mediabox-selectable', context).click(function () {
        var $item = $(this);
        if (!$item.hasClass('mediabox-selected')) {

          // Unlimited cardinality or cardinality limit is not reached for now.
          if(fieldCardinality === -1 || selected < fieldCardinality) {
            ++selected;
            $item.toggleClass('mediabox-selected');
          }
          else if (fieldCardinality === 1) {
            // For cardinality 1 we will unselect previous item and then select
            // current one.
            $('.mediabox-selectable.mediabox-selected', context).toggleClass('mediabox-selected');
            $item.toggleClass('mediabox-selected');
          }
          else {
            // @todo - we are not handling > 1 cardinality in UI. For this to
            // work we would need to pass additional data - how many mediabox
            // items are currently added.
          }
        } else {
          --selected;
          $item.removeClass('mediabox-selected');
        }
      });
    });
  }
};

/**
 * Recenter and resize mediabox dialog.
 */
function reajustMediaboxDialog($dialog) {
  var $w = $(window);

  // Resize dialog content height/width if we have room and there is a need for
  // it - we always fit dialog in viewport.
  var $content = $dialog.find('.ui-dialog-content')
  var newHeight = $w.height() * 0.75;
  if ($content.height() > newHeight) {
    $content.height(newHeight);
  }
  var newWidth = $w.width() * 0.85;
  if ($content.width() > newWidth) {
    $content.width(newWidth);
  }

  // Top center position of dialog.
  $dialog.css('top', $w.scrollTop() + $w.height() * .05);
  $dialog.css('left', ($w.width() - $dialog.width()) / 2);
}

/**
 * Mediabox dialog command.
 */
Drupal.ajax.prototype.commands.mediaboxDialog = function (ajax, response, status) {
  // Store options in global object so we can access it later, in behaviors.
  // @todo - this global storage is not the smartest idea, but too tiered to
  // try to fix it now.
  Drupal.mediabox.settings = response.settings;

  var $data = $(response.data);

  var $dialogContent = $('<div class="mediabox-dialog-content"></div>');
  $dialogContent.append($data);

  var $mediaboxDialog = $('<div title="' + response.options.title + '"></div>');
  $mediaboxDialog.append($dialogContent);

  var $dialog;

  // Dialog options.
  var options = {
    modal: true,
    width: 'auto',
    dialogClass: 'mediabox-dialog',
    open: function(event) {
      // Trigger open callback if it exist.
      var callback = Drupal.mediabox[response.options.openCallback];
      if (callback) {
        callback($(this).parent(), response.options);
      }

      // If ajax event is related to dialog content change this can also change
      // it size so we need to reajust it.
      var $this = $(this);
      $(document).bind('ajaxComplete.mediaboxDialog', function() {
        reajustMediaboxDialog($this.parent());
      });
    },
    close: function(event) {
      Drupal.detachBehaviors($dialog);

      $(document).unbind('ajaxComplete.mediaboxDialog');

      $(this).dialog('destroy');

      // Dialog destroy will not remove it content, we need to do it.
      $mediaboxDialog.remove();
    }
  };

  options.buttons = [];
  for (var buttonId in response.options.buttons) {
    if (response.options.buttons.hasOwnProperty(buttonId)) {
      var button = response.options.buttons[buttonId];
      options.buttons.push({
        text: button.text,
        id: buttonId,
        class: button.class,
        href: button.href,
        click: function(e) {
          // Trigger button callback if it exist.
          var callback = Drupal.mediabox[response.options.buttons[e.currentTarget.getAttribute('id')].callback];
          if (callback) {
            callback($(this).parent(), response.settings);
          }

          // By default all buttons close dialog.
          $(this).dialog('close');
        }
      });
    }
  }

  $dialog = $mediaboxDialog.dialog(options);

  // Store in global also because of callbacks support and our behaviors.
  Drupal.mediabox.dialog = $dialog;

  // Trigger behaviors.
  Drupal.attachBehaviors($dialog);

  // Behaviors can change size of dialog content (for example plupload) so we
  // need to reajust it.
  reajustMediaboxDialog($dialog.parent());
};

/**
 * Mediabox close dialog command. Close current open mediabox dialogs.
 */
Drupal.ajax.prototype.commands.mediaboxCloseDialog = function (ajax, response, status) {
  if (Drupal.mediabox.dialog) {
    Drupal.mediabox.dialog.dialog('close');
  }
};

/**
 * Mediabox add files command.
 *
 * Take passed mediabox entity ids in response.ids and add them ass mediabox
 * displays.
 */
Drupal.ajax.prototype.commands.mediaboxAddFiles = function (ajax, response, status) {
  if (response.ids.length > 0) {
    // Trigger hidden form interface for adding new mediabox displays.
    $('#' + Drupal.mediabox.settings.field + '_mid_elements').attr('value', response.ids.join());
    $('#' + Drupal.mediabox.settings.field + '_add_more').trigger('mousedown');
  }
  // Lets close plupload dialog.
  Drupal.mediabox.dialog.dialog('close');
};

})(jQuery);
