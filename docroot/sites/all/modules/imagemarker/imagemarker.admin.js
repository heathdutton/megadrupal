
(function($) {
  
  /**
   * Override ImageMarker object.
   */  
  
  // Add editProperties.
  Drupal.ImageMarker.prototype.editProperties = function () {

    var self = this;
    if ($('#imagemap-edit').find('.imagemap-edit-x').length == 0) {
      this.attachForm();
    }
    $('input[name="imagemap-edit-x"]').val(this.x);
    $('input[name="imagemap-edit-y"]').val(this.y);
    $('textarea[name="imagemap-edit-description"]').val(this.description);
          
  };
  
  // Add attachForm.
  Drupal.ImageMarker.prototype.attachForm = function() {

    // Empty the box first.
    $('#imagemap-edit').empty();

    $('#imagemap-edit')
      .append($('<input readonly="readonly" name="imagemap-edit-x" class="imagemap-edit-x" />'))
      .append($('<input readonly="readonly" name="imagemap-edit-y" class="imagemap-edit-y" />'))
      .append($('<label>' + Drupal.t('Description') + '</label>'))
      .append($('<textarea name="imagemap-edit-description" class="imagemap-edit-description text-full form-textarea"></textarea>'))
      .append($('<input type="button" class="imagemap-button" name="imagemap-edit-submit" value="' + Drupal.t('Save') + '" />')
        .click(function() {
          Drupal.ImageMap.saveCoordinates();
          $('#imagemap-edit').empty();
          $('#imagemap-edit').append(Drupal.t('Marker has been saved.'));
        }))
      .append($('<input type="button" class="imagemap-button" name="imagemap-edit-remove" value="' + Drupal.t('Remove') + '" />')
        .click(function() {
          Drupal.ImageMap.removeCoordinate();
          $('#imagemap-edit').empty();
          $('#imagemap-edit').append(Drupal.t('Marker has been removed.'));
        }));
  };

  /**
   * Drupal behaviors.
   */
  Drupal.behaviors.ImageMapAdmin = {

    attach : function(context, settings) {
      // Administration sections will display the coordinates on the right
      // and change the markers into clickable pointers.
      Drupal.ImageMap.init();
    }
  };
    
})(jQuery);
