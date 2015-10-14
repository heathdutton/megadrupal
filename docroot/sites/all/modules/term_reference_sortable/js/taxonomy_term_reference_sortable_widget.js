(function($) {
  Drupal.behaviors.termReferenceSortable = {
    attach: function(context, settings) {
      //init select
      $('.sortable2', context).parents('.field-suffix').siblings('select').hide();

      $('.sortable2', context).once('sort-init').each(function(index, value){

        var $this = $(this);
        var $select = $this.parents('.field-suffix').siblings('select');
        if($select.hasClass('error')){
          $this.addClass('error');
        }

        $select.find('option').each(function(index, value){
         var  $option = $(value);
          if($option.val() != '_none'){

            if($(value).attr('selected')){
              $this.append('<li class="ui-state-highlight"  id="' + $option.val() + '">' + $option.html() + '</li>');
            } else {
              $this.parent().find('.sortable1').append('<li class="ui-state-default"  id="' + $option.val() + '">' + $option.html() + '</li>');
            }
          }
        });
      });

      $(".sortable1, .sortable2", context).once('sort-attach').sortable({
        connectWith: ".connectedSortable"
      }).disableSelection();

      $(".sortable2", context).once('sort2-attach').sortable({
        receive:function(event, ui){
          var max_elements = $(event.target).parent('.sortable-lists').attr('max_elem');
          if(max_elements){
            if($(event.target).sortable('toArray').length > max_elements){
              $(ui.sender).sortable('cancel');
              var $sortable_list  = $(event.target).parents('.sortable-lists');
              if($sortable_list.find('.error-message').length < 1){
                $sortable_list.prepend('<div class="error-message">Maximum limit: ' + max_elements + '</div>');
              }
            }
          }

        },
        remove:function(event, ui){
          var required = $(event.target).parent('.sortable-lists').hasClass('required');
          if (required) {
            if($(event.target).sortable('toArray').length < 1){
              var $sortable_list  = $(event.target).parents('.sortable-lists');
              if($sortable_list.find('.error-message.minimum').length < 1){
                $sortable_list.prepend('<div class="error-message minimum">This field is required, add At least one value!</div>');
              }
            }
          }
        }
      });

      $(".sortable2", context).once('sort-bind').bind("sortupdate", function(event, ui) {
        sortSelect(this);
      });

      function sortSelect(element){
        var selected = $(element).sortable('toArray');
        var $select = $(element).parents('.field-suffix').siblings('select');
          //for different versions of jQuery
          if (typeof jQuery.prop != 'undefined') {
              $select.find(':selected').prop("selected", "");
          } else {
              $select.find(':selected').attr("selected", "");
          }

        if($(selected).length > 0){
          $(selected.reverse()).each(function(index, value){
            var $curent_option = $select.find('option:[value="'+ value +'"]');
            var $curent_option_clone = $curent_option.clone().attr("selected", "selected");
            $curent_option.remove();
            $select.prepend($curent_option_clone);

          });
        }
      }

    }
  };

})(jQuery);
