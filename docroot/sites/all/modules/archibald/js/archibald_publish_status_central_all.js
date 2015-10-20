var archibald_publish_all_urls = {};
var current_lom_id = "";
Drupal.behaviors.archibald = {
  attach: function (context, settings) {
   archibald_publish_check_status_all_init();
  }
}

function archibald_publish_check_status_all_init() {
  var resources = Drupal.settings.archibald_publish_all_resources;
  var post_array = {};
  var count = 0;
  for(var i in resources) {
    if(!resources.hasOwnProperty(i)) {
      continue;
    }
    post_array['resources[' + (count++) +']'] = resources[i];
  }

  archibald_publish_check_status_all(post_array);
  archibald_publish_ajax_cron_all();

  jQuery("#to_publish_frame_all > table td > a").each(function() {
    var link = jQuery(this).attr('href');
    current_lom_id = jQuery(this).attr('id').split("_", 2)[1];
    jQuery(this).attr('href', 'javascript:void(0);');
    jQuery(this).click(function() {
      var archibald_modal_translate_finish = archibald_modal_defaults;
      archibald_modal_translate_finish.modalSize = {width:600, height:400, type:'absolute'};
      Drupal.CTools.Modal.show(archibald_modal_translate_finish);

      jQuery.ajax({
          type: 'GET',
          url: link,
          success: function(response, status) {

            for (var i in response) {
              if (response[i]['command'] && Drupal.ajax.prototype.commands[response[i]['command']]) {
                Drupal.ajax.prototype.commands[response[i]['command']](Drupal.ajax, response[i], status);
              }
            }

            //Stop status check timeout couse we clicked on the close link
            jQuery('#modalContent > div.ctools-modal-content > .modal-header > a').click(function() {
              if(jQuery("#modal-title", jQuery(this).parent()).html() == Drupal.t('Detail status')) {
                archibald_publish_stop_status_check(current_lom_id);
              }
            });
          },
          dataType: 'json'
      });
    });
  });
}

function archibald_publish_check_status_all(post_array) {
  var message_template = '<div class="publish_status_message message_%color%"><b>%type%:</b> %message%</div>';

  jQuery.ajax({
    type: 'POST',
    url: Drupal.settings.archibald_publish_all_resources_urls.status_check,
    data: post_array,
    success: function(response, status) {
      for(var id in response) {
        if(!response.hasOwnProperty(id)) {
          continue;
        }

        if (id == 'messages') {
          jQuery('#publish_status_messages').html("");
          var messages = response[id];
          for(var index in messages) {
            if(!messages.hasOwnProperty(index)) {
              continue;
            }

            var message = messages[index];
            var msg = message_template;
            if (message.type == 'warning') {
               msg = msg.replace(/%type%/, Drupal.t('Status'));
               msg = msg.replace(/%color%/, 'yellow');
            }
            else if (message.type == 'success') {
               msg = msg.replace(/%type%/, Drupal.t('Success'));
               msg = msg.replace(/%color%/, 'green');
            }
            else if (message.type == 'error') {
               msg = msg.replace(/%type%/, Drupal.t('Error'));
               msg = msg.replace(/%color%/, 'red');
            }

            jQuery('#publish_status_messages').append(msg.replace(/%message%/, message.message));
          }
        }
        else if (id == 'status') {
          jQuery("#publish_status_main_progress .progress .bar .filled").css('width', '0%');
          jQuery("#publish_status_main_progress .progress .percentage").css('width', '0%');
          jQuery("#publish_status_main_progress .progress .message").html(response[id]);
        }
        else {
          jQuery('#'+id).html(response[id]);
        }
      }
      window.setTimeout(function() {
        archibald_publish_check_status_all(post_array);
      }, 2000);
    },
    dataType: 'json'
  });
}

function archibald_publish_ajax_cron_all() {
  jQuery.ajax({
    type: 'GET',
    url: Drupal.settings.archibald_publish_all_resources_urls.cron_check + '?run_cron=1',
    success: function(response, status) {
      window.setTimeout(function(){
        archibald_publish_ajax_cron_all();
      }, 1000);
    },
    dataType: 'json'
  });
}