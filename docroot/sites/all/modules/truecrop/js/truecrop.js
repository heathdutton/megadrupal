(function($) {

/**
 * Integrates the Manual Crop user interface with True Crop.
 */
Drupal.behaviors.trueCrop = {
  // Determines if the crop interface should be automatically reopened after
  // the next Ajax request.
  reopen: false,

  // Tracks the name of the image style that was used in the most recent crop
  // session.
  cropStyle: '',

  // Tracks the ID of the file that was used in the most recent crop session.
  cropFid: 0,

  // Tracks the ID of the file that should be reverted to when the file was
  // swapped during a cropping session and the user decides to undo those
  // changes. This is equivalent to the file that was originally used when the
  // crop session was started.
  revertFid: 0,

  attach: function (context) {
    var truecrop = this;

    // Automatically open the most recently used cropping interface when
    // requested.
    if (truecrop.reopen) {
      // The crop widget may not be on the page immediately after an Ajax
      // request, so keep trying until it appears.
      var reopen_attempt = setInterval(function () {
        var $crop_widget = truecrop.cropWidget();
        if ($crop_widget.length) {
          // Depending on how Manual Crop is configured, the element that opens
          // the cropping interface can be a thumbnail or button (both of which
          // respond to the "mousedown" event) or a select element which
          // responds to the "change" event (see _manualcrop_add_croptool()),
          // so attempt to trigger all of those.
          var $button_or_thumbnail = $crop_widget.find('.manualcrop-style-button, .manualcrop-style-thumb');
          if ($button_or_thumbnail.length) {
            $button_or_thumbnail.mousedown();
          }
          else {
            $crop_widget.find('.manualcrop-style-select').val(truecrop.cropStyle).change();
          }
          clearInterval(reopen_attempt);
          truecrop.reopen = false;
        }
      }, 150);
    }

    // Handle the "Save" and "Cancel" buttons within the cropping interface.
    // This catches all clicks on the HTML document and searches for the
    // desired button within those, since somewhere down the line Manual Crop
    // prevents the mousedown event from propagating.
    $('body').once('truecrop-submit').mousedown(function (event) {
      // Handle the "Save" button.
      if ($(event.target).hasClass('manualcrop-close')) {
        // Trigger a True Crop replacement for the image that was just cropped,
        // but only if there is actually a new crop being saved. This prevents
        // unnecessary crops from being created.
        if (truecrop.newCropExists($(event.target))) {
          truecrop.triggerTrueCropReplacement();
        }
        truecrop.endCropSession();
      }
      // Handle the "Cancel" button.
      if ($(event.target).hasClass('manualcrop-cancel')) {
        // If the image was swapped out during this crop session, restore the
        // original image.
        if (truecrop.revertFid && truecrop.revertFid != truecrop.cropFid) {
          truecrop.replaceFile(truecrop.revertFid);
        }
        truecrop.endCropSession();
      }
    });

    $('body').once('truecrop-menu-tree')
      // Handle clicking on one of the related crop images.
      .on('click', '.truecrop-menu-tree .truecrop-related-crop', function (event) {
        event.preventDefault();
        // Don't do anything if the cropping interface is still being opened
        // from a previous click.
        if (truecrop.reopen) {
          return false;
        }
        // Don't do anything if the requested file is the one that is already
        // being edited.
        var newFid = $(this).data('fid');
        if (newFid == truecrop.cropFid) {
          return false;
        }
        // Track the previous file, if this is the first replacement made
        // during this cropping session.
        if (!truecrop.revertFid) {
          truecrop.revertFid = truecrop.cropFid;
        }
        // Indicate that the cropping interface should be opened after the file
        // is replaced, then replace the file. Also add an Ajax throbber so the
        // user is aware that the interface is in the process of being
        // replaced.
        truecrop.reopen = true;
        $('<div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div></div>').insertAfter($(this));
        truecrop.replaceFile(newFid);
        return false;
      })

      // Handle clicking on the related crop show/hide toggle.
      .on('click', '.truecrop-menu-toggle-link', function () {
        // Set the width in CSS to prevent the crop interface from shrinking or
        // growing horizontally as it expands and collapses vertically.
        var fixWidth = function ($container) {
          if (!$container.data('truecrop-width-fixed')) {
            // Add an extra pixel to the width to prevent rounding errors which
            // can cause text to spill over to the next line.
            $container.css('width', ($container.width() + 1) + 'px');
            // Only do this once to prevent extra pixels from continually being
            // added.
            $container.data('truecrop-width-fixed', 1);
          }
        }
        if ($(this).data('truecrop-toggle-action') == 'show') {
          fixWidth($(this).closest('.truecrop-related-crops'));
          // Expand the interface and change the toggle label.
          $(this).closest('fieldset').find('> .fieldset-wrapper').slideDown({
            duration: 'fast',
            easing: 'linear'
          });
          $(this).text(Drupal.t('hide'));
          $(this).data('truecrop-toggle-action', 'hide');
        }
        else {
          fixWidth($(this).closest('.truecrop-related-crops'));
          // Collapse the interface and change the toggle label.
          $(this).closest('fieldset').find('> .fieldset-wrapper').slideUp({
            duration: 'fast',
            easing: 'linear'
          });
          $(this).text(Drupal.t('show'));
          $(this).data('truecrop-toggle-action', 'show');
        }
        return false;
      });

    // Add the related crop show/hide toggle.
    $(context).find('.truecrop-related-crops legend')
      .once('truecrop-menu-tree-toggle')
      .append($('<span class="truecrop-menu-tree-toggle">(<a class="truecrop-menu-toggle-link" href="#">' + Drupal.t('hide') + '</a>)</span>'));
  },

  /**
   * Returns the widget used during the most recent (or current) crop session.
   */
  cropWidget: function () {
    return $('#manualcrop-area-' + this.cropFid + '-' + this.cropStyle).closest('.truecrop-wrapper');
  },

  /**
   * Determines if a new crop was created during this editing session.
   *
   * @param $button
   *   A jQuery object representing the "Save" button that was clicked to end
   *   the editing session.
   *
   * @return bool
   */
  newCropExists: function ($button) {
    // Check the presence of the "Revert selection" button in the cropping
    // interface to see if a new crop exists. This checks for display:none
    // rather than :visible since the button may not be visible when this code
    // runs (the parent cropping interface may have already been closed).
    return $button.siblings('.manualcrop-reset').first().css('display') != 'none';
  },

  /**
   * Triggers a crop and Ajax file replacement for the current crop selection.
   */
  triggerTrueCropReplacement: function () {
    var $crop_widget = this.cropWidget();
    $crop_widget.find('.truecrop-style').val(this.cropStyle);
    $crop_widget.find('.truecrop-crop-and-replace-button').mousedown();
  },

  /**
   * Replaces the current file being cropped with a different file.
   *
   * @param int fid
   *   The file ID of the new file to use.
   */
  replaceFile: function (fid) {
    // Find the crop widget based on the file that is currently being cropped.
    var $crop_widget = this.cropWidget();
    // Then replace the file and trigger a rebuild of the widget; also remove
    // the previous crop selections before rebuilding so the new file uses the
    // default values.
    this.cropFid = fid;
    $crop_widget.find('.fid').val(fid);
    $crop_widget.find('.manualcrop-cropdata').remove();
    $crop_widget.find('.truecrop-rebuild-button').mousedown();
  },

  /**
   * Do tasks specific to True Crop when the crop session ends.
   */
  endCropSession: function () {
    // If the file was switched out during this crop session, delete the record
    // of the original file (since with the end of the crop session the
    // replacement has either been saved or undone).
    this.revertFid = 0;
  }
};

/**
 * When the crop tool is shown, record the file and style that triggered it.
 */
Drupal.behaviors.trueCrop.showCroptoolOriginal = ManualCrop.showCroptool;
ManualCrop.showCroptool = function(identifier, style, fid) {
  // Get the style name using the same technique that the original Manual Crop
  // function does.
  if (typeof style == 'string') {
    var styleName = style;
  }
  else {
    var styleSelect = $(style);
    var styleName = styleSelect.val();
  }

  // Store the style name and file ID.
  Drupal.behaviors.trueCrop.cropStyle = styleName;
  Drupal.behaviors.trueCrop.cropFid = fid;

  // Call the original Manual Crop function.
  return Drupal.behaviors.trueCrop.showCroptoolOriginal(identifier, style, fid);
};

})(jQuery);
