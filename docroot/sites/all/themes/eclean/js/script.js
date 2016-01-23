
(function ($) {
  Drupal.behaviors.eclean = {
    attach: function (context, settings) {
      // Check the width on load nad reorder columns if needed
      checkWidth('');
      // Check width on window resize
      $(window).resize(function() {
        checkWidth('resize');
      });
      // Check the width of the window and run reorder_container()
      function checkWidth(event) {
        docwidth = $(document).width();
        if (docwidth > 905) {
          reorder_container(1140, event);
        }
        else if ((docwidth <= 905) && ($('body').hasClass('two-sidebars'))) {
          reorder_container(920, event);
        }
        else if ((docwidth <= 765) && ($('body').hasClass('sidebar-first'))) {
          reorder_container(920, event);
        }
      }

      // Reorder rhe elements in #container depending on window width
      function reorder_container(width, event) {
        var layout = new Array();
        var content = new Array();
        // Define the order of the element for different widths
        layout['1140'] = ['sub-header', 'left', 'center', 'right'];
        layout['920'] = ['sub-header', 'center', 'left', 'right'];
        
        // Get the content for each child element
        content['sub-header'] = $('#container #sub-header');
        content['left'] = $('#container #left');
        content['center'] = $('#container #center');
        content['right'] = $('#container #right');
        
        var grp = $("#container").children();
        $(grp).remove();

        for(var key in layout[width]) {  
          $("#container").append(content[layout[width][key]]);
        }

        if (event == 'resize') {
      //          window.location.reload()
        }
      }  
    }
  }
}(jQuery));










