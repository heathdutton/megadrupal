//Drupal.behaviors.audiorecorderfield = function (context) {
jQuery(document).ready(function($) {
  Recorder.initialize({
        swfSrc: Drupal.settings.soundcloud_recorder[0]
  });
  
  $.fn.extend({
        center: function () {
            return this.each(function() {
                var top = ($(window).height() - $(this).outerHeight()) / 2;
                var left = ($(window).width() - $(this).outerWidth()) / 2;
                $(this).css({position:'absolute', margin:0, top: (top > 0 ? top : 0)+'px', left: (left > 0 ? left : 0)+'px'});
            });
        }
}); 
});

function timecode(ms) {
        var hms = {
          h: Math.floor(ms/(60*60*1000)),
          m: Math.floor((ms/60000) % 60),
          s: Math.floor((ms/1000) % 60)
        };
        var tc = []; // Timecode array to be joined with '.'
        if (hms.h > 0) {
          tc.push(hms.h);
        }
        tc.push((hms.m < 10 && hms.h > 0 ? "0" + hms.m : hms.m));
        tc.push((hms.s < 10  ? "0" + hms.s : hms.s));
        return tc.join(':');
      }
    

      

      function record(field_id){
        jQuery("#soundcloud-"+field_id+"-wrapper .soundcloud-record").hide();
        jQuery("#soundcloud-"+field_id+"-wrapper .soundcloud-stop").show();
        Recorder.record({
          start: function(){
            //alert("recording starts now. press stop when youre done. and then play or upload if you want.");
          },
          progress: function(milliseconds){
            jQuery("#time-"+field_id).html(timecode(milliseconds));
          }
        });
      }
      
      function play(field_id){
        jQuery("#soundcloud-"+field_id+"-wrapper .soundcloud-play").hide();
        jQuery("#soundcloud-"+field_id+"-wrapper .soundcloud-pause").show();
        //Fix bug when sometimes pause button is shown as display:inline
        jQuery("#soundcloud-"+field_id+"-wrapper .soundcloud-pause").css('display','block');
        //Recorder.stop();
        Recorder.play({
          finished: function(){               
            // will be called when playback is finished
           jQuery("#soundcloud-"+field_id+"-wrapper .soundcloud-pause").hide();
           jQuery("#soundcloud-"+field_id+"-wrapper .soundcloud-play").show();
          },
          progress: function(milliseconds){
            if (jQuery("#soundcloud-"+field_id+"-wrapper .soundcloud-pause").is(":visible")) {
              jQuery("#time-"+field_id).html(timecode(milliseconds));
            }
          }
        });
      }
      
      function stop(field_id){
        jQuery("#soundcloud-"+field_id+"-wrapper .soundcloud-stop").hide();
        jQuery("#soundcloud-"+field_id+"-wrapper .soundcloud-play").show();
        jQuery("#soundcloud-"+field_id+"-wrapper .record-again-btn").show();
        Recorder.stop();
      }
      
      function pause(field_id){
        jQuery("#soundcloud-"+field_id+"-wrapper .soundcloud-pause").hide();
        jQuery("#soundcloud-"+field_id+"-wrapper .soundcloud-play").show();
        Recorder.stop();
      }
      
      function upload(field_name, langcode, delta, field_raw) {
        var field_id = field_name+"-"+langcode+"-"+delta;
        var file_id = field_raw+'['+langcode+']['+delta+'][fid]';
        
        //Remove any error messages
	    jQuery("#edit-field-"+field_id+"-wrapper .error").remove();
	    //Disable the button
	    jQuery("#soundcloud-button-"+field_id).attr('disabled', 'disabled');
        if(jQuery("#soundcloud-button-"+field_id).val()=='Upload') {
	     //Upload button
         var path=Drupal.settings.basePath +"?q=audiorecorderfield_file_receive";
         Recorder.upload({
              url: path, 
              audioParam: "track[asset_data]",
              params: {
                "track[title]":   "recorder.js track test",
                "track[sharing]": "private"
              },
              success: function(fid){
               if (fid == null || fid == "") {
		         jQuery("#soundcloud-button-"+field_id).removeAttr("disabled");
		         jQuery("#edit-field-"+field_id+"-wrapper").append('<div class="messages error file-upload-js-error">Failed to submit the voice recording!</div>');
		       }
               else {
                 jQuery.ajax({
					type: "GET",
					url: Drupal.settings.basePath +"?q=audiorecorderfield_file_preview",
					data: "fid=" + fid+"&field_id=" + field_id,
					dataType: 'json',
					success: function (data) {
                        SoundcloudPreview(data, field_id);
                    }
				});
                 //jQuery("#edit-field-"+field_id+"-fid").val(fid);
                 jQuery('input[name="'+file_id+'"]').val(fid);
               }               
              }
            });
        }
        else {
	      //jQuery("#edit-field-"+field_id+"-fid").val("");
          jQuery('input[name="'+file_id+'"]').val("");
	      jQuery("#soundcloud-button-"+field_id).removeAttr("disabled");
	      jQuery("#soundcloud-button-"+field_id).val('Upload');
          jQuery.ajax({
					type: "GET",
					url: Drupal.settings.basePath +"?q=audiorecorderfield_recorder_reload",
					data: "langcode="+langcode + "&delta=" + delta +  "&field_raw=" + field_raw,
					dataType: 'json',
					success:  function (data) {
                      jQuery("#audiorecorderfield-"+field_id+"-wrapper").html(data.recorder);
                    }
				});
        }        
      }
      
      function record_again(field_id) {
        jQuery("#soundcloud-"+field_id+"-wrapper .soundcloud-pause").hide();
        jQuery("#soundcloud-"+field_id+"-wrapper .soundcloud-play").hide();
        jQuery("#soundcloud-"+field_id+"-wrapper .soundcloud-stop").hide();
        jQuery("#soundcloud-"+field_id+"-wrapper .record-again-btn").hide();
        jQuery("#soundcloud-"+field_id+"-wrapper .soundcloud-record").show();
        jQuery("#time-"+field_id).html(timecode(0));
      }
      
function SoundcloudPreview(data, field_id){
  //Remove Upload button and add Remove button
  jQuery("#soundcloud-button-"+field_id).removeAttr("disabled");
  jQuery("#soundcloud-button-"+field_id).val('Remove');
  jQuery("#soundcloud-"+field_id+"-wrapper").remove();
  jQuery("#soundcloud-button-"+field_id).before(data.player);
}      
