/**
 * @file
 * Preview the feeds csv file.
 */

(function ($) {
  'use strict';
  Drupal.behaviors.feeds_csv_preview_before_import = {
    attach: function (context) {
      var inputrad;
      jQuery(".feeds-file-upload .description").html('<p style="color:red">Upload CSV File from local system to see the Preview of the CSV File.</p>');
      jQuery(".feeds-file-upload .descriptiontitle").show();
      jQuery("input#edit-feeds-feedsfilefetcher-upload").change(function (e) {
        var ext = jQuery("input#edit-feeds-feedsfilefetcher-upload").val().split(".").pop().toLowerCase();
        if (jQuery.inArray(ext, ["csv"]) !== -1) {
          if (e.target.files !== 'undefined') {
            var reader = new FileReader();
            reader.onload = function (e) {
              var csvval = e.target.result.split("\n");
              var totalrows = csvval.length - 1;
              inputrad = "<div class=previewtitle><span class=lefttitle>preview</span><span class=righttitle>Total number of records :" + "<span class=countrecords>" + totalrows + "</span></span></div>";
              inputrad = inputrad + "<table class=previewtable>";
              for (var k = 0; k < 10; k++) {
                inputrad = inputrad + "<tr class=previewrow>";
                var csvvalue = csvval[k].split(",");
                for (var i = 0; i < csvvalue.length; i++) {
                  var temp = csvvalue[i];
                  inputrad = inputrad + "<td class=previewcolumn>" + temp + "</td>";
                }
                inputrad = inputrad + '</tr>';
                jQuery(".feeds-file-upload .description").html(inputrad);
                jQuery(".feeds-file-upload .descriptiontitle").show();
              }
              inputrad = "</table>";
            };
            reader.readAsText(e.target.files.item(0));
            jQuery(".description").css("width", "auto");
          }
          return false;
        }
      });
    }
  };
})(jQuery);
