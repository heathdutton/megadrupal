(function ($) {
  last_batch_operation = "";
  last_batch_operation_title = "";
  archibald_batch_cron_success = true;
  archibald_batch_cron_fatal = false;
  function batch_action_check() {
    $.ajax({
      type: "POST",
      dataType: 'json',
      url: Drupal.settings.archibald_batch_ajax,
      data: {
        batch_op_ids: $('input[name=batch_op_ids]').val(),
        batch_op: last_batch_operation
      },
      success: function(data) {

        // Remove previous status.
        $('#archibald_admin_content_overview .batch_op_status').remove();

        // No errors so redirect to the confirmation page.
        if (data.success) {
          $('input[name=redirect]').val(window.location.href)
          $('#archibald-admin-content-batch-ops-form')[0].submit();
        }
        // Error handling.
        else {
          $('#batch_op_notification').find('h3.checking').hide();
          var errors = $('#batch_op_notification').find('h3.error').html(Drupal.t('The action «@action» cannot be carried out on some of the selected descriptions.',{
            '@action': last_batch_operation_title
          }));

          if (data.main_error) {
            errors.append('<span>' + data.main_error + '</span>');
          }
          errors.fadeIn();

          $('#batch_op_notification').find('h4.error_explain').fadeIn();
          $('.batch_op_status').remove();
          $.each(data.ids, function(key, value) {

            // Skip entries which have no error.
            if (!value.error) {
              return;
            }

            $('#archibald_admin_content_overview input:checkbox[value="' + key + '"]')
            .attr('checked', false) // Set the checkbox
            .parent().next() // Get the text table column
            .append($('<div class="batch_op_status"><ul><li>' + value.msg.join('</li><li>') + '</li></ul></div>'));
          });
          $('input[name=header_checkbox]:checked').attr('checked', false);
        }
      },
      error: function(XHR) {
        if (XHR.status == 403) {
          alert(Drupal.t('Access denied'));
        }
      }

    })
  };

  function archibald_batch_submit() {
    $('#archibald_batch_operation_submit').hide();
    $('#batch_op_notification').find('.hide').hide();
    $('#batch_op_notification .in_progress').fadeIn();
    $('#batch_op_notification .wait').fadeIn();
    $('.batch_op_status').html(Drupal.t('Working...'));

   // $(form).hide();
    if (last_batch_operation == "") {
      last_batch_operation = $('input[name="batch_op"]').val();
    }
    $.ajax({
      type: "POST",
      dataType: 'json',
      url: Drupal.settings.archibald_batch_submit_url,
      data: {
        batch_op_ids: $('input[name=batch_op_ids]').val(),
        batch_op: last_batch_operation,
        op: 'confirm'
      },
      success: function(data){

        $.each(data.valid_ids, function(key, value){
          $('#' + value).html((data.cron) ? $('.bo_working').text() : $('.bo_done').text());
        });

        $.each(data.invalid_ids, function(key, obj){
          $('#' + obj.lom_id).html('<ul><li>' + obj.msg.join('</li><li>') + '</li></ul>');
        });

        if (data.cron) {
            archibald_batch_cron(data.batch_op_key, data.batch_op_time);
            archibald_batch_check_status(data.batch_op_key, data.batch_op_time);
        }
        else {
          if (data.invalid_ids.length) {
            $('#batch_op_notification .in_progress').hide();
            $('#batch_op_notification .wait').hide();
            $('#batch_op_notification .done').fadeIn();
            $('#batch_op_notification .errors').fadeIn();
            $('#batch_op_notification .batch_op_redirect').fadeIn();
          }
          else {
            $('#batch_op_notification .in_progress').hide();
            $('#batch_op_notification .wait').hide();
            $('#batch_op_notification .done').fadeIn();
            $('#batch_op_notification .batch_op_redirect').fadeIn();
            window.location.href = $('.batch_op_redirect > a').attr('href');
          }
        }
      },
      error: function(XHR) {
        if (XHR.status == 403) {
          alert(Drupal.t('Access denied'));
        }
      }
    });
  }

  function archibald_batch_cron(key, time) {
    // We are finished or have a fatal error, disable cron.
    if (archibald_batch_cron_success || archibald_batch_cron_fatal) {
      return;
    }
    $.ajax({
      type: "POST",
      dataType: 'json',
      url: Drupal.settings.archibald_batch_submit_url,
      data: {
        batch_op_key: key,
        batch_op_time: time,
        op: 'cron'
      },
      success: function(){
        setTimeout(function(){
          archibald_batch_cron(key, time)
        }, 1000);
      },
      error: function(XHR) {
        if (XHR.status == 403) {
          alert(Drupal.t('Access denied'));
        }
      }
    });
  }
  function archibald_batch_check_status(key, time) {
    $.ajax({
      type: "POST",
      dataType: 'json',
      url: Drupal.settings.archibald_batch_submit_url,
      data: {
        batch_op_key: key,
        batch_op_time: time,
        batch_op_lom_ids: $('input[name=batch_op_ids]').val(),
        op: 'status'
      },
      success: function(data){
        archibald_batch_cron_fatal = data.fatal || data.all_errors;
        $.each(data.results, function(lom_id, result) {
          var message = Drupal.t('Done');
          if (result == 'error') {
            archibald_batch_cron_success = false;
            message = Drupal.t('Error');
          }
          else if (result == 'waiting') {
            message = Drupal.t('Waiting');
          }
          $('#' + lom_id).html(message);
        });

        if (data.finished) {
          $('#batch_op_notification .in_progress').hide();
          $('#batch_op_notification .wait').hide();
          $('#batch_op_notification .done').fadeIn();
          $('#batch_op_notification .complete').fadeIn();
          $('#batch_op_notification .batch_op_redirect').fadeIn();
          if (archibald_batch_cron_success) {
            window.location.href = $('.batch_op_redirect > a').attr('href');
          }
        }
        else if (data.fatal === false && data.all_errors === false) {
          setTimeout(function(){
            archibald_batch_check_status(key, time)
          }, 1000);
        }
        else if (data.fatal === true || data.all_errors === true) {
          $('#batch_op_notification .in_progress').hide();
          $('#batch_op_notification .wait').hide();
          if (data.fatal === true) {
            $('#batch_op_notification .fatalerror').fadeIn();
          }
        }
      },
      error: function(XHR) {
        if (XHR.status == 403) {
          alert(Drupal.t('Access denied'));
        }
      }
    })
  }



  Drupal.behaviors.archibald_batch_ops = {
    attach: function () {
      $('#batch_op_notification').find('.hide').hide();
      $('#batch_op_notification').css('display', 'block');

      // Batch operation change event. (submit)
      $('#edit-batch-op-select').change(function(){

        if ($(this).val() == '') {
          return;
        }
        archibald_batch_cron_success = true;

        // Save the last batch operation value and title because we set the selectbox entry to the first one again
        // and we would loose the choosen action.
        last_batch_operation = $(this).val();
        last_batch_operation_title = $('option[value="' + $(this).val() + '"]', $(this)).html().replace("&gt; ", "");

        $('#archibald_admin_content_overview .batch_op_status').remove();


        // Check selected checkboxes.
        var ids = $('input[name=batch_op_ids]').val( // Set the value of the input.
          $('.archibald_batch_chk:checked') // Get all selected checkboxes
          .map(function() { return this.value; }) // Setup a map function to produce an array.
          .get() // Get the array based up on the map function.
          .join(',') // Join the array into a comma seperated string.
          .trim() // Remove whitespaces at the end and front.
        )
        .val(); // Return the value.

        // If we have selected entries, check the given descriptions.
        if (ids != '') {
          $('#batch_op_notification').find('h3.checking').fadeIn();
          $('#batch_op_notification').find('h3.error').hide();
          $('#batch_op_notification').find('h4.error_explain').hide();
          batch_action_check();
        }

        // Set the selectbox back to the first entry.
        $(this).val('');

        // Restore the value.
        $('.batch_operation_form input[name="batch_op"]').val(last_batch_operation);
      });

      // Multi checkbox event.
      $('#archibald_batch_multicheck').unbind('change').bind('change', function() {
        $('.archibald_batch_chk').attr('checked', $(this).attr('checked'));
      });

      // Submit form
      $('#archibald_batch_operation_submit').unbind('click').bind('click', function() {
        archibald_batch_submit();
      });
    }
  };

})(jQuery);