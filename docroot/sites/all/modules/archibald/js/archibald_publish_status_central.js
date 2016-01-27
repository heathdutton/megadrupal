var archibald_publish_urls = {};
var archibald_publish_status_timeouts = {};
function archibald_publish_check_status(lom_id) {
  if(archibald_publish_urls[lom_id] == undefined) {
    return;
  }
  var url = archibald_publish_urls[lom_id] + '?ajax=1';
  jQuery.ajax({
    type: 'GET',
    url: url,
    success: function(response, status) {
      jQuery('.middlecontainer .widecolumn .messages').remove();
      if (response.messages.length > 0) {
        jQuery('.middlecontainer .widecolumn')
          .prepend(response.messages);
      }
      jQuery('#to_publish_frame').html(response.content);
      archibald_publish_status_timeouts[lom_id] = window.setTimeout(function() {
        archibald_publish_check_status(lom_id);
      }, 2000);
    },
    dataType: 'json'
  });
}

function archibald_publish_stop_status_check(lom_id) {
  if (archibald_publish_status_timeouts[lom_id] != undefined) {
    clearTimeout(archibald_publish_status_timeouts[lom_id]);
  }
}

function archibald_publish_ajax_cron(lom_id) {
  if(archibald_publish_urls[lom_id] == undefined) {
    return;
  }
  var url = archibald_publish_urls[lom_id] + '?run_cron=1';
  jQuery.ajax({
    type: 'GET',
    url: url,
    success: function(response, status) {
      window.setTimeout(function(){
        archibald_publish_ajax_cron(lom_id);
      }, 3000);
    },
    dataType: 'json'
  });
}

function archibald_publish_start(lom_id, cron) {
  archibald_publish_check_status(lom_id);
  if(cron == '1') {
    archibald_publish_ajax_cron(lom_id);
  }
}