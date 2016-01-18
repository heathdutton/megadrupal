(function ($) {
  Drupal.behaviors.dominionMenuAdmin = {
    attach: function(context) {
      if ($('#edit-initial-links').length) {
        $('#edit-initial-links input').click(function() {
          Drupal.dominionMenuAdminChangeOptions();
        });
        Drupal.dominionMenuAdminChangeOptions();
      }
      if ($('.form-item-dominion-menu-copy').length) {
        $('.form-item-dominion-menu-copy label:first').after($('<div class="form-item form-checkboxes"><input type="checkbox" id="dominion-checkall" class="form-checkbox"> <label class="option" style="font-style:italic" for="dominion-checkall">' + Drupal.t('Check all') + '</label></div>'));
        $('#dominion-checkall').click(function() {
          $('.form-item-dominion-menu-copy input').attr('checked', $(this).attr('checked'));
        });
      }
    }
  };
  
  Drupal.dominionMenuAdminChangeOptions = function() {
    if ($('input#edit-initial-links-default').attr('checked')) {
      $('.form-item-default-links').show();
      $('.form-item-copy-source').hide();
    }
    if ($('input#edit-initial-links-copy').attr('checked')) {
      $('.form-item-default-links').hide();
      $('.form-item-copy-source').show();
    }
  }
})(jQuery);
