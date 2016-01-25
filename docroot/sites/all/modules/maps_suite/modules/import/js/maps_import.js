(function ($) {

  Drupal.behaviors.maps_import = {
    attach: function (context, settings) {
      /**
       * Function for auto selecting criteria in 'at most' if they are selected in 'at least'.
       */
      $('#edit-at-least option').on('click', function(){
        var value = $(this).val();

        var target = $('#edit-at-most:visible').find('option[value="' + value + '"]');
        target.attr('selected', 'selected');
      });

      $('#edit-disable-at-least').on('click', function(){
        if ($('#edit-disable-at-most').attr('checked')){
          return false;
        }

        $('.form-item-at-least').toggle();
        $('#edit-at-least').find('option:selected').attr('selected', false);
      });


      $('#edit-disable-at-most').on('click', function(){
        if ($('#edit-disable-at-least').attr('checked')){
          return false;
        }

        $('.form-item-at-most').toggle();
        $('#edit-at-most').find('option:selected').attr('selected', false);
      });
    }
  };

}(jQuery));


