(function ($) {
  Drupal.behaviors.knockoutBracket = {
    attach: function (context, settings) {
      $('#knockout-bracket-wrapper', context).once('knockout-bracket', function () {
        // Add drag scrolling
        $('#knockout-bracket-wrapper').dragscrollable({dragSelector: '#knockout-bracket'});

        // Drag cursors
        $('#knockout-bracket')
        .mousedown(function() {
          $(this).addClass('grabbing');
        })
        .mouseup(function() {
          $(this).removeClass('grabbing');
        });
        // Participant highlighting
        $('#knockout-bracket-wrapper .participant').hover(
          function() {
            var id = $(this).attr('participant-id');
            if (id != '0') {
              $(".participant-" + id).addClass('hover');
            }
          },
          function() {
            var id = $(this).attr('participant-id');
            if (id != '0') {
              $(".participant-" + id).removeClass('hover');
            }
          }
        );

        // Toggle knockout tabs
        $('#knockout-bracket-tabs a').click(function() {
          var location = $(this).attr('location');

          // Set the tab active
          $('#knockout-bracket-tabs a').removeClass('active-pane');
          $(this).addClass('active-pane');

          // Set the tab contents active
          $('div.knockout-bracket-pane').removeClass('active-pane');
          $('div.knockout-bracket-pane.' + location).addClass('active-pane');
          return false;
        });

        // Link action
        $('#knockout-bracket-actions .bracket-link a').click(function() {
          var selector = $('#knockout-bracket-link');
          selector.toggle();

          $(this).toggleClass('clicked');

          // Ensure clicks outside of the menu hide it
          $('#knockout-bracket').mousedown(function(event) {
            var target = $(event.target);
            if (target.parents('#knockout-bracket-link').length == 0) {
              selector.hide().removeClass('clicked');
            }
          });

          return false;
        });
        
        // Close rankings link
        $('#knockout-close-rankings').click(function() {
          $('#knockout-rankings').hide();
          return false;
        });

        // Link textfields
        $('#knockout-bracket-link .form-text').click(function() {
          $(this).select();
        });
      });
    }
  };
})(jQuery);
