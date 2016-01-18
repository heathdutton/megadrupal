(function ($) { // http://stackoverflow.com/questions/4681309/using-jquery-in-drupal-7


  $(document).ready(
    function () {
      // Get the node id from where we left it in the header
      // var nodeid=$("input#edit-nid").val(); # WORKED but we actually don't need node ID - so let's use some static number:
      var nodeid=9999999;
      // alert(Drupal.settings.basePath); // Contains "/" if Drupal is installed inthe root!
      // load the current state of the checkboxes once on loading page
      $.getJSON(Drupal.settings.basePath + "bgchecklist/loadall/"+nodeid,
        function(json) {
          for( i=0; i < json.length; i++ ) {
            if (json[i].state == "1") {
              $("#"+json[i].qid).attr("checked","true").next().html('<font color=red>invisible</font>');
            } else {
              $("#"+json[i].qid).removeAttr("checked").next().html('<font color=green>visible</font>');
            }
          }
        });

      // Setup an onclick for each checkbox that writes it state back to the database
      // when toggled. The label text is turned red while writing to the db.
      $("input.form-brilliant_gallery_checklist-checkbox").each(
        function () {
          $(this).click(
            function () {
              var thislabel=$(this).next();
              var colorbefore=$(thislabel).css("color");
              //alert("/bgchecklist/save/"+nodeid+"/"+$(this).attr("id")+"/1");
              if ( $(this).attr("checked") == false ) {
                $(thislabel).html("saving...").css("color","red");
                $.get(Drupal.settings.basePath + "bgchecklist/save/"+nodeid+"/"+$(this).attr("id")+"/0",
                  function() {
                    $(thislabel).html('<font color=green>visible</font>').css("color","green");
                  });
              } else {
                $(thislabel).html("saving...").css("color","red");
                $.get(Drupal.settings.basePath + "bgchecklist/save/"+nodeid+"/"+$(this).attr("id")+"/1",
                  function() {
                    $(thislabel).html('<font color=red>invisible</font>').css("color","red");
                  });
              }
            });
        });
    }
    );

})(jQuery);
