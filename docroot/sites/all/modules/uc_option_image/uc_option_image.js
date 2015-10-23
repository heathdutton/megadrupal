/**
 * @file
 * Show the selected option
 */
(function ($) {
  Drupal.UCOptionImage = Drupal.UCOptionImage || {};

  Drupal.UCOptionImage.selected = [];

  /**
   * Initialize the form, and show any images for default selections
   */
  Drupal.UCOptionImage.init = function() {
    Drupal.UCOptionImage.update();

    $('*[id^=edit-attributes]').bind('change', function() {
      Drupal.UCOptionImage.update();
    });
  };

  /**
   * Add a selected option
   */
  Drupal.UCOptionImage.add_selected = function(aid, oid) {
    if (!Drupal.UCOptionImage.selected[aid]) {
      Drupal.UCOptionImage.selected[aid] = [];
    }
    if (!Drupal.UCOptionImage.selected[aid][oid]) {
      Drupal.UCOptionImage.selected[aid][oid] = true;
    }
  };

  /**
   * Update the selected images, as necessary
   */
  Drupal.UCOptionImage.update = function() {
    Drupal.UCOptionImage.selected = [];

    Drupal.UCOptionImage.update_inputs();
    Drupal.UCOptionImage.update_selects();

    Drupal.UCOptionImage.show_images();
  }

  /**
   * Update the image for this attribute if the checkbox / radio is selected
   */
  Drupal.UCOptionImage.update_inputs = function(el) {
    $('input[id^=edit-attributes]:checked').each(function() {
      var match = $(this).attr('id').match(/edit\-attributes\-(\d+)\-(\d+)/);
      if (!match) {
        return;
      }

      var aid = match[1];
      var oid = match[2];

      Drupal.UCOptionImage.add_selected(aid, oid);
    });
  };

  /**
   * Update the image for this attribute if the select item is chosen
   */
  Drupal.UCOptionImage.update_selects = function(el) {
    $('select[id^=edit-attributes]').each(function() {
      var match = $(this).attr('id').match(/edit\-attributes\-(\d+)/);
      if (!match) {
        return;
      }

      var aid = match[1];
      var oid = $(this).val();

      Drupal.UCOptionImage.add_selected(aid, oid);
    });
  };

  /**
   * Display this option image.  If the option is already displayed, it remains visible.
   *
   * Multiple choices for the same option will show each choice in the order they are chosen.
   */
  Drupal.UCOptionImage.show_images = function() {
    for (var aid in Drupal.UCOptionImage.selected) {
      if (!aid) {
        continue;
      }

      var el = $('#uc-option-image-selected-' + aid);
      if (el.length < 1) {
        continue;
      }

      el.html('');
      Drupal.UCOptionImage.show_image(aid);
    }
  };

  /**
   * Add images for a given aid
   */
  Drupal.UCOptionImage.show_image = function(aid) {
    var options = Drupal.settings['uc_option_image-' + aid];
    if (!options) {
      return;
    }

    var el = $('#uc-option-image-selected-' + aid);
    if (el.length < 1) {
      return;
    }

    for (oid in Drupal.UCOptionImage.selected[aid]) {
      if (!oid) {
        continue;
      }
      if (!options[oid]) {
        continue;
      }

      el.append(options[oid]);
    }
  }

  Drupal.behaviors.uc_option_image = {
    attach: function(context, settings) {
      Drupal.UCOptionImage.init();
    }
  }
})(jQuery);
