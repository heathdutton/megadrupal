(function ($) {

"use strict";

/**
 * Constructor.
 *
 * @param {jQuery} $field
 *   Mediabox field for we are attaching better UI.
 * @param {object} settings
 *   Settings better UI object.
 * @returns
 */
var MediaboxBetterUI = function($field, settings) {
  this.$field = $field;
  this.settings = settings;
  
  // Selected thumb delete button.
  this.$deleteButton = $('<input class="form-submit btn btn-warning" type="button" value="Select items to delete">');

  if (parseInt(settings.fieldCardinality) === 1) {
    this.mediaboxOneItem($field);
  }
  else {
    this.mediaboxMultipleItems($field);
  }
};

/**
 * Handle delete button click event.
 *
 * @param $element
 *   jQuery element that can have deleted state.
 * @param $field
 *   jQuery element that has appropriate checkbox.
 */
MediaboxBetterUI.prototype.deleteButtonClicked = function($element, $field) {
  $element.toggleClass('mediabox-deleted');
  
  // When checkbox for appropriate element is checked/unchecked 
  // it is directly marked for delete and will be processed in php later.
  var $checkbox = $field.find('.mediabox-remove').find('.form-type-checkbox input');
  $checkbox.attr('checked', !$checkbox.is(':checked'));
 
  this.deleteButtonText($element);
};

/**
 * Update delete button text based on selected thumb status.
 *
 * @param $element
 *   jQuery element that can have deleted state.
 */
MediaboxBetterUI.prototype.deleteButtonText = function($element) {
  if ($element.hasClass('mediabox-deleted')) {
    this.$deleteButton.val(this.settings.restoreItemText);
  }
  else {
    this.$deleteButton.val(this.settings.deleteItemText);
  }
};

/**
 * Show/update crop info in thumbnail.
 *
 * @param {jQuery} $thumb
 *   Thumbnail image jquery object.
 * @param {jQuery} $image
 *   Full size image jquery object.
 * @param {jQuery} $item
 *   Item jquery object that is holding crop input fields for this image.
 */
MediaboxBetterUI.prototype.updateCropInfo = function ($thumb, $image, $item) {
  var $thumbImage = $thumb.find('img');

  var xScale = $thumbImage.width() / $image.width();
  var yScale = $thumbImage.height() / $image.height();
  var x = parseInt($item.find('input.jcrop-x').attr('value')) * xScale;
  var y = parseInt($item.find('input.jcrop-y').attr('value')) * yScale;
  var x1 = parseInt($item.find('input.jcrop-x1').attr('value')) * xScale;
  var y1 = parseInt($item.find('input.jcrop-y1').attr('value')) * yScale;
  // Its not very effective to use JCrop here but it is a quick solution.
  // If perfromace are problem we can create our own crop info tool based
  // on code from JCrop.
  if ($thumbImage.data('Jcrop')) {
    $thumbImage.data('Jcrop').setSelect([x, y, x1, y1]);
  } else {
    $thumbImage.Jcrop({
      setSelect: [x, y, x1, y1],
      allowSelect: false,
      allowMove: false,
      allowResize: false
    });
  }
};

/**
 * Create snapshot of values of all input elements in passed $item.
 *
 * @param {jQuery} $item
 *   Item we wanna snapshot.
 */
MediaboxBetterUI.prototype.snapshotItem = function($item) {
  $item.find('input, textarea').each(function () {
    $(this).data('mediabox-snapshot', $(this).val());
  });
};

/**
 * Undo values of all input elements in passed $item from previous snapshot.
 *
 * @param {jQuery} $item
 *   Item we wanna undo.
 */
MediaboxBetterUI.prototype.undoItem = function($item) {
  // We do not support all form elements, just the most common ones.
  $item.find('input, textarea').each(function () {
    var $input = $(this);
    if (typeof $input.data('mediabox-snapshot') !== 'undefined') {
      $input.val($input.data('mediabox-snapshot'));
    }
  });
};

/**
 * Reset(undo) crop widget to values from hidden inputs.
 *
 * @param {jQuery} $item
 *   Item we wanna reset crop widget.
 */
MediaboxBetterUI.prototype.resetCrop = function($item) {
  var x = $item.find('input.jcrop-x').attr('value');
  var y = $item.find('input.jcrop-y').attr('value');
  var x1 = $item.find('input.jcrop-x1').attr('value');
  var y1 = $item.find('input.jcrop-y1').attr('value');
  $item.find('img').data('Jcrop').setSelect([x, y, x1, y1]);
};

/**
 * Field with multiple items (cardinality more then 1) handling.
 *
 * @param $field
 *   jQuery field object.
 */
MediaboxBetterUI.prototype.mediaboxMultipleItems = function($field) {
  var mediaboxUI = this;

  // Dont use tr selector directly because new popup item edit feature is
  // actually changing table tr content.
  var $mediaboxContainer = $field.find('.field-multiple-table tbody');

  var getItem = function (index) {
    return $mediaboxContainer.find('tr')[index];
  };

  // Holds selected thumbnail.
  var $selectedThumb = null;

  // Create new thumbnails sortable control element.
  var $thumbsWrapper = $('<tr class="mediabox-better-ui-thumbs-wrapper"><td colspan="3"><div class="mediabox-better-ui-thumbs"></div></td></tr>');
  var $thumbs = $thumbsWrapper.find('.mediabox-better-ui-thumbs');
  //$thumbs.empty();
  $field.find('.field-multiple-table thead').append($thumbsWrapper);

  // Move 'Add new items' button on top of thumbnails control. We can have
  // multiple descriptions here (from file uploads during ajax adding) so we
  // target last one which is always our description - little hackish but it
  // works OK.
  $field.find('.description').last().insertBefore($thumbs);

  // Move 'Add new items' button on top of thumbnails control.
  $field.find('.mediabox-add-items').parent().insertBefore($thumbs);

  // Put delete button below thumbs.
  this.$deleteButton.insertAfter($thumbs);
  this.$deleteButton.click(function () {
    // @todo - for now we are not handling a case when some items are
    // deleted but then user add new images - we will loose information
    // about deleted images.
    // $selectedThumb.toggleClass('mediabox-deleted');
    $thumbsWrapper.find('.mediabox-selected').each(function (index) {
      $(this).toggleClass('mediabox-deleted');
      // Drag&Drop adds opacity property which makes element visible even with class mediabox-deleted.
      $(this).css('opacity', '');
      var $item = $(getItem($(this).data('mediabox-item-index')));
      mediaboxUI.deleteButtonClicked($item, $item);
    });
  });

  // Process items.
  $mediaboxContainer.find('tr').each(function (index) {
    var $item = $(this);
    var $image = $item.find('img.mediabox-image');

    // If we do not have image element in this item lets return.
    if ($image.size() === 0) {
      return;
    }

    // Items are hidden by default, visibility is controled by thumbnails.
    $item.hide();

    // Create thumbnail from mediabox item images.
    var $thumb = $('<div class="mediabox-image"><img /></div>');
    // We can have two images from Jcrop so always take the first one.
    $thumb.find('img').attr('src', $image.get(0).src);

    // Save item index position in thumbnail for thumbnail operations.
    $thumb.data('mediabox-item-index', index);

    // On thumbnail click show mediabox item for this thumbnail.
    $thumb.click(function () {
      $item.show();
      $thumb.toggleClass('mediabox-selected');
      
      $selectedThumb = $thumb;
      mediaboxUI.deleteButtonText($item);
    });

    if (mediaboxUI.settings.itemInPopup === 1) {
      $mediaboxContainer.hide();
      $thumb.dblclick(function () {
        mediaboxUI.snapshotItem($item);

        var $dialogContent = $('<table id="mediabox-popup-content"></table>');
        $dialogContent.append($item);

        var $mediaboxDialog = $('<div title="' + mediaboxUI.settings.popupTitle + '"></div>');
        $mediaboxDialog.append($dialogContent);

        $($mediaboxDialog).dialog({
          resizable: true,
          modal: true,
          width: 'auto',
          position: {
            using: function(pos) {
              var $w = $(window);
              // Top of the dialog we will set based on current viewport.
              $(this).css('top', $w.scrollTop() + $w.height() * .05);
              $(this).css('left', pos.left);
            }
          },
          buttons: [
            {
              text: 'Save', // @todo - should be translatable.
              id: 'mediabox-popup-save',
              click: function() {
                $item = $(this).contents().find('tr.draggable');
                mediaboxUI.updateCropInfo($thumb, $image, $item);
                $(this).dialog('close');
              }
            },
            {
              text: 'Cancel', // @todo - should be translatable.
              id: 'mediabox-popup-cancel',
              click: function() {
                mediaboxUI.undoItem($item);
                mediaboxUI.resetCrop($item);
                $(this).dialog('close');
              }
            }
          ],
          open: function(event) {
            // Resize dialog height if needed to 75% of viewport, we always
            // fit dialog in viewport.
            var newHeight = $(window).height() * 0.75;
            if ($(this).height() > newHeight) {
              $(this).height(newHeight);
            }
          },
          close: function(event) {
            // Dialog will take it content out of the DOM - that is why we
            // need to return content back to the DOM.
            $item = $(this).contents().find('tr.draggable');
            var position = $thumb.data('mediabox-item-index');
            if(position === 0) {
              $mediaboxContainer.prepend($item);
            }
            else {
              $mediaboxContainer.find('tr:nth-child(' + position + ')').after($item);
            }
          }
        });
      });
    }

    $thumbs.append($thumb);

    // Adding update crop information need to go after crop.
    // We are using setTimout here instead of direct call because some browsers
    // (Chrome) will not properly init whole JCrop control. 100 ms is picked
    // after couple of tries - 10-30ms was too little and 100ms looks big enough
    // to avoid this problem and again it is fast enough for UI.
    setTimeout(function () {
      mediaboxUI.updateCropInfo($thumb, $image, $item);
    }, 100);

    // We need this overlay when we use crop info in  thumbnails so sorting
    // can work properly - without it crop will take over dragging control.
    $thumb.append('<div class="mediabox-image-overlay"></div>');
    
    // First item is selected by default.
    //if (index === 0) {
    //  $thumb.click();
    //}
  });

  // If there are no images in list just hide delete button.
  ($thumbs.find('img').length === 0) ? this.$deleteButton.hide() : this.$deleteButton.show();

  // Thumbnails are sortable.
  $thumbs.sortable({
    distance: 3,
    opacity: 0.5,
    placeholder: 'ui-state-highlight mediabox-image',
    deactivate: function(event, ui) {
      // When thumbnail sorting is finished lets update items delta.
      $thumbs.find('.mediabox-image').each(function (index) {
        var itemIndex = $(this).data('mediabox-item-index');
        $('.form-delta-order', getItem(itemIndex)).val(index);
      });
    }
  });
};

/**
 * Field with one item (cardinality 1) handling.
 *
 * @param $field
 *   jQuery field object.
 */
MediaboxBetterUI.prototype.mediaboxOneItem = function($field) {
  var $element = this.$field.find('.mediabox-element');
  if(this.$field.find('img.mediabox-image').size() !== 0) {
    this.$deleteButton.insertBefore($element);
    var mediaboxUI = this;
    this.$deleteButton.click(function () {
      mediaboxUI.deleteButtonClicked($element, $field);
    });
    this.deleteButtonText($element);
  }
};

/**
 * Better mediabox field UI.
 */
Drupal.behaviors.mediaboxBetterUI = {
  attach:function (context, settings) {
    // Attach better UI functionality to all mediabox fields.
    if (typeof Drupal.settings.mediabox_ui === 'undefined') {
      return;
    }
    for (var field in Drupal.settings.mediabox_ui.fields) {
      if (Drupal.settings.mediabox_ui.fields.hasOwnProperty(field)) {
        $('#edit-' + field + ' .mediabox-form-wrapper', context).once('mediabox-better-ui', function () {
          var $field = $(this);

          // Hide default drag/remove control because we will use different solution.
          $field.find('.mediabox-remove').hide().next().attr('colspan', 3);
          
          new MediaboxBetterUI($field, Drupal.settings.mediabox_ui.fields[field]);
        });
      }
    }
  }
};

})(jQuery);
