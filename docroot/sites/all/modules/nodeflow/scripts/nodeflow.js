(function ($) {
  Drupal.behaviors.nodeflow = {
    attach: function (context, settings) {

      // Auto-submit the queue pull-down list on change.
      $('#queue-list select').bind('change', function() {
        var url = $(this).val();
        if (url) {
          window.location = url;
        }
        return false;
      });
      
      // This object will contain the pre-snapshot of locked rows in the nodeflow table.
      var presnap = {};
      var nodeArr = [];

      // Take a snapshot of the locked rows which returns an object containing nid and position pairs.
      var snapshot = function(event) {
        var lockedrows = {};
        nodeArr = [];
        var pos = 0;
        $('#nodeflow-published tbody tr:not(.ui-sortable-placeholder, .scheduled)').each(function() {
          pos++;
          if ($(this).hasClass('locked')) {
            var nid = $(this).find('.nid').val();
            lockedrows[nid] = pos;
            nodeArr.push(nid);
          }
        });
        return lockedrows;
      };


      /**
       * This function needs to be modified to take into account 'unscheduled' nodes.
       * @bug with restacking and locked positions around unscheduled nodes.
       */
      // The nodeflow position number assigning function.
      var posorder = function() {
        // Set special row color if scheduled time is set
        $('#nodeflow-published tbody tr.draft').each(function() {
          if ($(this).find('.schedule_date').val()) {
            $(this).closest('tr').removeClass('draft');
            $(this).closest('tr').addClass('scheduled');
          }
        });

        // Check if locked rows are displaced, and if so, revert them.
        var postsnap = snapshot('end');
        if (!$.isEmptyObject(presnap)){
          // Loop through locked rows.
          for (key in nodeArr) {
            nid = nodeArr[key];
            // Check if its position has been displaced.
            if (postsnap[nid] != presnap[nid]) {
              var curRow = $('.nid' + nid); 
              // Swap them.
              if ((postsnap[nid] - presnap[nid]) > 0) {
                // This happens when a new row is added into the right-side column.
                $(curRow).prev().before($(curRow));
              }
              else {
                // This happens when a row is removed from 
                // or moved further down in the right-side column.
                // Check if the next row is locked.
                if ($(curRow).next().hasClass('locked')) {
                  // If yes, then we cannot swap with a locked row, so 
                  // fetch the next available unlocked row and swap with it.
                  var nextId = $(curRow).nextAll(':not(.locked)').find('.nid').val();
                  // Go through all the subsequent locked rows and update their positions
                  // in the snapshot array so they dont need to perform any swaps as the
                  // previous swap should have put these subsequent locked rows into the right positions.
                  var tmpRow = curRow;
                  while ($(tmpRow).next().hasClass('locked')) {
                    var curId = $(tmpRow).next().find('.nid').val();
                    postsnap[curId] = postsnap[curId] + 1;
                    tmpRow = $('.nid' + curId);
                  }
                  // Make the actual swap now.
                  $(curRow).before($('.nid' + nextId));
                }
                else {
                  // No group locked rows so we can do a normal swap.
                  $(curRow).next().after($(curRow));
                }
              }
            }
          }
        }

        // Assign the position numbers.
        var pos = 1;
        $('#nodeflow-published tbody tr:not(.scheduled)').each(function() {
          $(this).find('.counter').text(pos);
          pos++;
        });
      };

      // Run the posorder once when the page loads to assign numbers to all the positions.
      posorder();

      // The drag and drop setup with jQuery UI sortable.
      $("#nodeflow-published tbody, #nodeflow-draft tbody").sortable({
        connectWith: '#nodeflow-published tbody',
        items: 'tr:not(.locked)',
        containment: '#page',
        handle: '.tabledrag-handle',
        start: function (event, ui) {
          presnap = snapshot('start');
        },
        update: function (event, ui) {
          ui.item.effect('highlight', {}, 3000);
          posorder();
        },
      }).disableSelection();
      
      $('.edit').click(function(event) {
        var link = $(event.target).find('a').attr('href');
        if (event.ctrlKey) {
          window.open(link);
        } else {
          document.location.href = link;
        }
      });
     
      // On clicking the lock/unlock icon toggle lock icon, set its hidden value, class name.
      $('.lock').click(function(event) {
        var cl = event.target;
        if ($('#pos' + cl.id).val() == '1') {
          $('#pos' + cl.id).val('0');
          $(this).toggleClass('on');
          $(cl).closest('tr').toggleClass('locked');
        }
        else {
          $('#pos' + cl.id).val('1');
          $(this).toggleClass('on');
          $(cl).closest('tr').toggleClass('locked');
        }
        return false;
      });

      // Remove row when clicked on delete icon.
      $('.remove').click(function(event) {
        var row = $(event.target).closest('tr');
        $(row).fadeOut('slow', function(){
          // Take a presnap of locked row positions before removing.
          presnap = snapshot('start');
          $(row).remove();
          posorder();
        });
        return false;
      });

      // Set datepicker on schedule date field.
      $('.schedule_date').datepicker({
        dateFormat: 'yy-mm-dd',
      });

      // Set timeEntry on schedule time field.
      $('.schedule_time').timeEntry({
        show24Hours: true,
      });

      // If wvegatimepicker plugin is available, use it.
      if ($.TimePicker) {
        $('.schedule_time').timepicker({
          timeFormat: 'HH:mm',
        });
      }

      // Overriding readonly input fields to be writable.
      $('.schedule_date, .schedule_time').click(function() {
        $(this).focus();
      });

      // Hide the status message after a few seconds.
      $('#console').delay(6000).hide('slow');

      // Move the submit button on top of the queue.
      $('#edit-actions').insertBefore('#nodeflow-published');

      // Polling to fetch realtime online users every 20 secs.
      var userpoll = function(){
        $.post('/admin/nodeflow/getusers', function(data) {
          $('.users-online ul').html('');
          for (var user in data) {
            $('.users-online ul').append('<li>' + user + '</li>');
          }
          setTimeout(userpoll, 20000);
        });
      };
      userpoll();
    }
  };
}(jQuery));
