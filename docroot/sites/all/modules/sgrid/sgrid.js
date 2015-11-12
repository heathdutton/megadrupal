(function ($) {
    Drupal.behaviors.sgrid = {
      attach: function (context, settings) {
             $(".sortable ul").sortable({
                update: function(event, ui) {
                    var ItemOrder = $(this).sortable('toArray').toString();   
                    $('#sgridorder').val(ItemOrder);
                    $(this).removeClass('sgrid-line-end'); 
                    sgrid_reset_eol($(this));
                },
                drag: function(event, ui) {
                    reset_eol($(this));                  
                },
                helper: 'clone',
                disabled: !(Drupal.settings.sgrid.sort_allowed)                                                
             });
            $(".sortable ul li").each(function (index, Element) {                    
                    $(this).attr('id', 'sgrid_item_' + $(this).find('span.sgrid-nid').text());
            });            
            // Initial order            
            var sort_items = []; 
            $(".sortable ul li").each(function (index, Element) {
                id = 'sgrid_item_' + $(this).find('span.sgrid-nid').text();
                $(this).attr('id', id);
                sort_items[index] = id;
            });                  
            $('#sgridorder').val(sort_items.toString());                
            $(".sortable ul li").disableSelection();
            // Ajax call for order saving in database
            $('#sgrid-button-save').click(function() {
              $(this).text('Saving ...');
              var ItemOrder = $(".sortable ul").sortable('serialize');   
              var save_request = $.ajax({
                method: 'GET',
                url: '/save_order/' + Drupal.settings.sgrid.view_name + '/' + Drupal.settings.sgrid.display_name,
                data: ItemOrder
              });
              save_request.done(function(msg) {
                $('#sgrid-mess').html(msg);
                $('#sgrid-button-save').text('Save');                
              });
            });
            $('#sgrid-button-reset').click(function() {
              $(this).text('Resetting order ...');
              var save_request = $.ajax({
                method: 'GET',
                url: '/save_order/' + Drupal.settings.sgrid.view_name + '/' + Drupal.settings.sgrid.display_name + '/1'
              });
              save_request.done(function(msg) {
                $('#sgrid-mess').html(msg);
                $('#sgrid-button-reset').text('Reset Order');
                location.reload();         
              });
            });
      }
    }
    
    function sgrid_reset_eol(obj) {
        // Reset end of line classes
        obj.find('li').each(function (index, Element) {
            $(this).removeClass('sgrid-line-end');
            var eof = index / Drupal.settings.sgrid.row_length;
            if (eof == parseInt(eof)) {
                $(this).addClass('sgrid-line-end');                           
            }
        });           
    }   
})(jQuery);

