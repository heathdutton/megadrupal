// $Id:

/**
* @file
* taskconsole.js
*/

/* When the task name in the task console is clicked, open the interactive task (inline function)
* Trigger the AJAX update to update the task start_date
*/
jQuery(function($) {
  var taskid = 0;
  $('.maestro_taskconsole_interactivetaskName a').click(function() {
    var taskid = jQuery(this).attr('taskid');
    $('#maestro_actionrec' + taskid).toggle();

    $.post(ajax_url + '/starttask/',"taskid=" + taskid + "&sec_token=" + sec_token);
    if (document.getElementById('maestro_actionrec' + taskid)) {
      $('html,body').animate({scrollTop: $('#maestro_actionrec' + taskid).offset().top - 125},500);
    }
  })

  /* If variable set, scroll down to the task and open it
     example use: after reviewing a form as part of an review form task,
     refresh the console and open the previous task back up.
  */
  scrollToTask = function(taskid) {
    $('html, body').animate({scrollTop: $('#maestro_taskcontainer' + taskid).offset().top}, 1000);
    $('#maestro_actionrec' + taskid).toggle();

  }

  if (typeof jump_taskid !== "undefined" &&jump_taskid > 0)
    if (document.getElementById('maestro_actionrec' + jump_taskid))
      scrollToTask(jump_taskid);

});




/* When the task name in the task console is clicked, open the interactive task (inline function)
* Trigger the AJAX update to update the task start_date
*/
jQuery(function($) {
  $('.maestro_taskconsole_viewdetail').click(function() {
    var taskid = jQuery(this).attr('taskid');
    var rowid = jQuery(this).attr('rowid');
    if (document.getElementById('maestro_taskconsole_detail_rec' + taskid).style.display == 'none') {
      $('#maestro_ajax_indicator' + taskid).show();
      $.ajax({
        type: 'POST',
        url : ajax_url + '/getdetails',
        cache: false,
        data : {
          taskid : taskid,
          rowid : rowid,
          sec_token : sec_token
        },
        dataType: 'json',
        success:  function (data) {
          if (data.status == 1) {
            // Swap the image of the closed folder for a open folder icon
            var s = $('#maestro_viewdetail_foldericon' + taskid).attr('src');
            var index = s.indexOf('_closed');
            var newicon = s.substr(0, index) + '_open' + s.substr(index + 7);
            $('#maestro_viewdetail_foldericon' + taskid).attr('src',newicon);
            $('#projectdetail_rec' + rowid).html(data.html);
            $('#maestro_taskconsole_detail_rec' + taskid).show();
            $('#maestro_ajax_indicator' + taskid).hide();
            $('a.ctools-use-modal:not(.ctools-use-modal-processed)')
            .addClass('ctools-use-modal-processed')
            .click(Drupal.CTools.Modal.clickAjaxLink)
            .each(function () {
              // Create a drupal ajax object
              var element_settings = {};
              if ($(this).attr('href')) {
                element_settings.url = $(this).attr('href');
                element_settings.event = 'click';
                element_settings.progress = { type: 'throbber' };
              }
              var base = $(this).attr('href');
              Drupal.ajax[base] = new Drupal.ajax(base, this, element_settings);
              // Attach the display behavior to the ajax object
            }
            );

          } else {
            alert(Drupal.t('An error occurred retrieving workflow details'));
          }
        },
        error: function() { alert(Drupal.t('there was a SERVER Error processing AJAX request')); }

      });

    } else {
      // Swap the image of the open folder for a closed folder icon
      var s = $('#maestro_viewdetail_foldericon' + taskid).attr('src');
      var index = s.indexOf('_open');
      var newicon = s.substr(0, index) + '_closed' + s.substr(index + 5);
      $('#maestro_viewdetail_foldericon' + taskid).attr('src',newicon);
      $('#maestro_taskconsole_detail_rec' + taskid).hide();
    }

  })
});


/* In the project details area, the workflow admin can change the assigned user for a task */
function maestro_ajaxUpdateTaskAssignment(id) {
  (function ($) {
    $.ajax({
      type: 'POST',
      url : ajax_url + '/setassignment',
      cache: false,
      data: $("#frmOutstandingTasksRow" + id).serialize(),
      dataType: 'json',
      success:  function (data) {
        if (data.status != 1) {
          alert(Drupal.t('An error occurred updating assignment'));
        } else {
          $("#assigned_user_row" + id).html(data.username);
        }
      },
      error: function() {
        alert(Drupal.t('there was a SERVER Error processing AJAX request'));
      }

    });
  })(jQuery);
}

/* In the project details area, the workflow admin can delete a project and its associated tasks and content */
function maestro_ajaxDeleteProject(id, message) {
  var x = confirm(message);
  if(x) {
    (function ($) {
      $.ajax({
        type: 'POST',
        url : ajax_url + '/deleteproject',
        cache: false,
        data: {tracking_id: id, sec_token : sec_token},
        dataType: 'json',
        success:  function (data) {
          alert(data.message);
          if (data.status == 1) {
            location.reload();
          }
        },
        error: function() {
          alert('there was a SERVER Error processing AJAX request');
        }
      });
    })(jQuery);
  }
}

function ajaxMaestroComment(op, rowid, id, cid) {
  if (op == 'new') {
    jQuery('#newcomment_container_' + rowid).show();
    jQuery('html,body').animate({scrollTop: jQuery('#newcomment_container_' + rowid).offset().top -50},500);
  } else if (op == 'add') {
    (function ($) {
      $.ajax({
        type : 'POST',
        url : ajax_url + '/add_comment',
        cache : false,
        data : {
          rowid : rowid,
          tracking_id : id,
          sec_token : sec_token,
          comment: document.getElementById("newcomment_" + id).value
        },
        dataType : 'json',
        success : function(data) {
          if (data.status == 1) {
            $('#projectCommentsOpen_rec' + rowid).html(data.html);
            $('html,body').animate({scrollTop: $('#projectCommentsOpen_rec' + rowid).offset().top-1},500);
          } else {
            alert(Drupal.t('An error occurred adding comment'));
          }
        },
        error : function() {
          alert(Drupal.t('there was a SERVER Error processing AJAX request'));
        }

      });
      $('#newcomment_container_' + rowid).hide();
    })(jQuery);

  } else if (op == 'del') {
    (function ($) {
      $.ajax({
        type : 'POST',
        url : ajax_url + '/del_comment',
        cache : false,
        data : {
          rowid : rowid,
          tracking_id : id,
          cid : cid,
          sec_token : sec_token
        },
        dataType : 'json',
        success : function(data) {
          if (data.status == 1) {
            $('#projectCommentsOpen_rec' + rowid).html(data.html);
            $('html,body').animate({scrollTop: $('#projectCommentsOpen_rec' + rowid).offset().top-1},500);
          } else {
            alert(Drupal.t('An error occurred deleting comment'));
          }
        },
        error : function() {
          alert(Drupal.t('there was a SERVER Error processing AJAX request'));
        }

      });
    })(jQuery);

  }

}

/*
* Function handles the form submit buttons for the inline interactive tasks All
* the form buttons should be of input type 'button' or 'submit' even the 'task complete'
* Function will fire automatically when a form button is pressed and execute
* the ajax operation for the interactive_post action and automatically post the
* form contents plus the taskid and task operation that was picked up from the
* button's custom 'maestro' attribute.
* example: <input type="button" maestro="save" value="Save Data">
*/
jQuery(function($) {
  $('.maestro_taskconsole_interactivetaskcontent input[type=submit], input[type=button]').click(function() {
    if (this.id == 'taskConsoleLaunchNewProcess') return;
    var id = jQuery(this).parents('tr').filter(".maestro_taskconsole_interactivetaskcontent").attr('id');
    if (id != undefined) {
      var idparts = id.split('maestro_actionrec');
      var taskid = idparts[1];
      var op = jQuery(this).attr('maestro');
      dataString = jQuery(this).closest('form').serialize();
      dataString += "&queueid=" + taskid;
      dataString += "&op=" + op;
      dataString += "&sec_token=" + sec_token;
      jQuery.ajax( {
        type : 'POST',
        cache : false,
        url : ajax_url + '/interactivetask_post',
        dataType : "json",
        success : function(data) {
          if (data.refresh == 1) {
            if (data.reopen_task == 1) {
              window.location = taskconsole_url + '/' + taskid;
            } else {
              window.location = taskconsole_url;
            }
          } else {
            $("#maestro_actionrec" + taskid).hide();
            if (data.status == 1) {
              if (data.hidetask == 1) {
                $("#maestro_taskcontainer" + taskid).hide();
                $("#maestro_taskconsole_detail_rec" + taskid).hide();
              }
            } else {
              alert(Drupal.t('An error occurred processing this interactive task'));
            }
          }
        },
        error : function() { alert(Drupal.t('there was a SERVER Error processing AJAX request')); },
        data : dataString
      });
      return false;
    }
  })
});


jQuery(function($) {
  $('#taskConsoleLaunchNewProcess').click(function() {
    $("#newProcessStatusRowSuccess").hide();
    $("#newProcessStatusRowFailure").hide();
    dataString = jQuery('#frmLaunchNewProcess').serialize();
    dataString += "&op=newprocess";
    dataString += "&sec_token=" + sec_token;
    jQuery.ajax( {
      type : 'POST',
      cache : false,
      url : ajax_url + '/newprocess',
      dataType : "json",
      success : function(data) {
        if (data.status == 1 && data.processid > 0) {
          $("#newProcessStatusRowSuccess").show();
        } else {
          $("#newProcessStatusRowFailure").show();
        }
      },
      error : function() {
        $("#newProcessStatusRowFailure").show();
      },
      data : dataString
    });
    return false;
  })
});


function toggleProjectSection(section,state,rowid) {
  var obj1 = document.getElementById(section + state + '_rec' + rowid);
  if (obj1) {
    if (state == 'Open') {
      obj1.style.display = 'none';
      var obj2 = document.getElementById(section + 'Closed_rec' + rowid);
      obj2.style.display = '';
    } else if (state = 'Closed') {
      obj1.style.display = 'none';
      var obj2 = document.getElementById(section + 'Open_rec' + rowid);
      obj2.style.display = '';
    }
  }
}

function projectDetailToggleAll(state,rowid) {
  jQuery(function($) {
    $(".taskdetailOpenRec" + rowid).each(function() {
      if (state == 'expand') {
        $(this).show();
      } else {
        $(this).hide();
      }
    })
    $(".taskdetailClosedRec" + rowid).each(function() {
      if (state == 'expand') {
        $(this).hide();
      } else {
        $(this).show();
      }
    })
    if (state == 'expand') {
      $("#expandProject" + rowid).hide();
      $("#collapseProject" + rowid).show();
    } else {
      $("#expandProject" + rowid).show();
      $("#collapseProject" + rowid).hide();
    }
  });
}

