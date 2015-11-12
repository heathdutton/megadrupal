/**
 * @file
 * Allows components to be sortable within their parent.
 */

(function ($) {

  Drupal.behaviors.AppleNewsSortableComponents = {
    attach: function(context, settings) {
      var storedContainerCount = 0;
      
      $(window).load(function() {
        reInitSortable();
        
        // When a new component container is added, re-initialize jQuery sortable
        $('#components-none-0').bind('DOMSubtreeModified', function() {
          var actualContainerCount = $('.components-container').length;
          
          if (actualContainerCount > storedContainerCount) {            
            storedContainerCount = actualContainerCount;
            
            reInitSortable();
          }
        });
      });
      
      function reInitSortable() {
        storedContainerCount = 0;
        
        $('.components-container').each(function() {
          storedContainerCount++;
          
          // Unbind existing sortable
          try {
            $(this).sortable('destroy');
          } catch(err) {
            // Suppress error - cannot call methods on sortable prior to initialization
            // Appears on first load
          }
          
          $(this).sortable({
            handle: ' > fieldset > legend > .fieldset-legend',
            stop: function(event, ui) {
              var parentId = $(this).parent().context.id;
              
              var componentType = $(event.target).attr('data-type');
              var parentCid = parentId.replace('components-' + componentType + '-', '');
              
              var serialized = $(this).sortable('serialize', {
                key: 'sort',
              });
              serialized = serialized.replace(/&/g, '-');
              
              // Save weights via ajax
              var location = window.location.pathname;
              var tid = location.substring(location.lastIndexOf('/') + 1);
              $.ajax({
                url: settings.basePath + 'publish-to-apple-news/ajax/weight/' + tid + '/' + componentType + '/' + parentCid + '/' + serialized
              });
              
              // Update the ids of each component
              var i = 1;
              $('#' + parentId + ' > div').each(function() {
                $(this).attr('id', 'components-' + componentType + '-' + parentCid + '-' + i);
                i++;
              });
            }
          });
        });
      }
    }
  };
  
}(jQuery));
