/*
  File name: taxon_taxonomy.js
  Version:   3.0

  Description:
  taxon_taxonomy is the proxy to allow Drupal to communicate
  with the Taxon system.
*/

/*
  taxon_taxonomy.js is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  taxon_taxonomy is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with taxon_taxonomy. If not, see <http://www.gnu.org/licenses/>.

  For more information read the README.txt file in the root directory.
*/
(function ($) {
  Drupal.behaviors.overlay_alter = {
    attach: function (context) {
      // Global settings from Drupal
      var taxonomy_name = Drupal.settings.taxon_taxonomy.taxonomy_name;
      var taxonomy_tag_name = Drupal.settings.taxon_taxonomy.field_name;
      var taxonomy_selected_image = Drupal.settings.taxon_taxonomy.selected_image;
      var taxonomy_not_selected_image = Drupal.settings.taxon_taxonomy.not_selected_image;
      var taxonomy_help_image = Drupal.settings.taxon_taxonomy.help_image;
      var taxonomy_waiticon_image = Drupal.settings.taxon_taxonomy.wait_image;
      var taxonomy_button_text = Drupal.settings.taxon_taxonomy.button_text;
      var taxonomy_button_text_wait = Drupal.settings.taxon_taxonomy.button_text_wait;
      var taxonomy_text_title = Drupal.settings.taxon_taxonomy.text_title;
      var taxonomy_text_breadcrumb = Drupal.settings.taxon_taxonomy.text_breadcrumb;
      var taxonomy_text_field = Drupal.settings.taxon_taxonomy.text_field;
      var taxonomy_text_field_ckeditor = Drupal.settings.taxon_taxonomy.text_field_ckeditor;
      var taxonomy_help_texts = Drupal.settings.taxon_taxonomy.help_texts;
      var taxonomy_auto_fill = Drupal.settings.taxon_taxonomy.auto_fill;
      var taxonomy_auto_fill_on_save = Drupal.settings.taxon_taxonomy.auto_fill_on_save;
      var taxonomy_delay_taxon_call = Drupal.settings.taxon_taxonomy.delay_taxon_call;
      var taxonomy_hide_taxon = Drupal.settings.taxon_taxonomy.hide_taxon;

      if (taxonomy_auto_fill) {
        /*
          Drupal.behaviors fires before the HTML of the overlay, especially CKEditor, is inserted into the DOM, so we need some
          sort of waiting mechanism.
        */
        setTimeout(function() {
          getTaxonResult(taxonomy_name, taxonomy_tag_name, taxonomy_text_field, taxonomy_text_field_ckeditor, taxonomy_button_text, taxonomy_button_text_wait, taxonomy_waiticon_image, taxonomy_delay_taxon_call, taxonomy_text_title, taxonomy_text_breadcrumb);
        }, taxonomy_delay_taxon_call);
      }

      $(document).find('#edit-submit').click(function() {
        if (taxonomy_auto_fill_on_save) {
          getTaxonResult(taxonomy_name, taxonomy_tag_name, taxonomy_text_field, taxonomy_text_field_ckeditor, taxonomy_button_text, taxonomy_button_text_wait, taxonomy_waiticon_image, taxonomy_delay_taxon_call, taxonomy_text_title, taxonomy_text_breadcrumb);
        }
      });

      if (! taxonomy_hide_taxon) {
        // Insert the button, but only once
        var test = $('div').find('#taxon-classify-button');

        if (test.length == 0) {
          $(taxonomy_tag_name).after("<div> <input style = 'margin-top:10px;' id = 'taxon-classify-button' class = 'form-submit ajax-processed' type='button' value='" + taxonomy_button_text + "'></div>");
        }

        // Get the text from the textarea
        $('div').find('#taxon-classify-button').click(function() {
          $('div').find('#taxon-classify-button').attr('value', taxonomy_button_text_wait);
          $('div').find('#taxon-classify-button').after("<img id = 'taxon-wait-cursor' src = '" + taxonomy_waiticon_image + "'>");

          $("#taxon-results").remove();

          var text = "";

          if (taxonomy_text_title != "") {
            text = $(taxonomy_text_title).val() + " ";
          }

          if (taxonomy_text_breadcrumb == true) {
            menu_item_text = $("#edit-menu-link-title").val();
            text += menu_item_text + " ";

            menu_item_index = $('#edit-menu-parent option:selected').index();

            menu_item_text = $('#edit-menu-parent option').eq(menu_item_index).text();

            while(menu_item_text.indexOf("----") >= 0) {
              text += menu_item_text + " ";

              menu_item_index--;

              menu_item_text = $('#edit-menu-parent option').eq(menu_item_index).text();
            }

            text += menu_item_text + " ";
          }

          // Get the text from the textarea
          if (taxonomy_text_field_ckeditor) {
            // Remove the starting #
            taxonomy_text_field_name = taxonomy_text_field.replace(/^\#/, '');

            CKEDITOR.config.entities = false;
            CKEDITOR.config.entities_latin = false;

            try {
              text += CKEDITOR.instances[taxonomy_text_field_name].getData();
            }
            catch(e) {
              alert("Warning from taxon_taxonomy: The field '" + taxonomy_text_field_name + "' does not exist. \nThe text from the CKEditor was not included.\nPlease check the settings for taxon_taxonomy.");
            }
          }
          else {
            text += $(taxonomy_text_field).val();
          }

          $.ajaxSetup({async:false});

          // Post to the Drupal proxy function and get the result back as text
          $.post("/taxon-taxonomy", { taxonomy: taxonomy_name, text: text }, function(data) {
            $('#taxon-classify-button').before("<div id = 'taxon-results'> </div>");

            var classids_string = "," + $(taxonomy_tag_name).val() + ",";

            var classids = [];

            var re = /\,\s*([0-9][0-9\.]*(?:[0-9]| ))/g;
            var match = {};

            while (match = re.exec(classids_string)) {
              classids.push(match[1].replace(/^\s+|\s+$/g, ''));
            }

            var lines = data.split("\n");

            for (line in lines) {
              if (lines[line] != "") {
                if (lines[line].match(/^Note\: /)) {
                  var append_text = "<div class = 'taxon-results-note-wrapper'><div class = 'taxon-results-note'>";
                  append_text += lines[line];
                  append_text += "</div></div>";
                  $('#taxon-results').append(append_text);
                }
                else {
                  // Get the ID
                  var matches = lines[line].match(/^([0-9\.]+)/);

                  // For CSS and files names we need to convert . to -
                  var id = "";
                  var css_id = "";

                  if (matches != null) {
                    id = matches[0];
                    css_id = id.replace(/\./g, "-");
                  }

                  if (jQuery.inArray(id, classids) != -1) {
                    // The classid is selected in the dropdown
                    var append_text = "<div class = 'taxon-results-result'><span class = 'taxon-state'><img src = '" + taxonomy_selected_image + "' class = 'state'></span><input style = 'margin-top:10px;' id = 'taxon-classify-" + line + "' class = 'taxon-result' type='button' value='" + lines[line] + "'>";

                    if (taxonomy_help_texts != "") {
                      append_text += "<span id = '" + css_id + "' class = 'taxon-help'><img src = '" + taxonomy_help_image + "' class = 'help'></span></div>";
                    }

                    $('#taxon-results').append(append_text);
                  }
                  else {
                    // The classid is NOT selected in the dropdown
                    var append_text = "<div class = 'taxon-results-result'><span class = 'taxon-state'><img src = '" + taxonomy_not_selected_image + "' class = 'state'></span><input style = 'margin-top:10px;' id = 'taxon-classify-" + line + "' class = 'taxon-result' type='button' value='" + lines[line] + "'>";

                    if (taxonomy_help_texts != "") {
                     append_text += "<span id = '" + css_id + "' class = 'taxon-help'><img src = '" + taxonomy_help_image + "' class = 'help'></span></div>";
                    }

                    $('#taxon-results').append(append_text);
                  }
                }
              }
            }

            $('.taxon-result').click(function() {
              if ($(this).siblings("span").children(".state").attr('src') != taxonomy_selected_image) {
                // Add the class
                $(this).siblings("span").children(".state").attr('src', taxonomy_selected_image);
                
                var class_title = $(this).val();

                var classes = $(taxonomy_tag_name).val();

                if (classes == "") {
                  $(taxonomy_tag_name).val(class_title);
                }
                else {
                  $(taxonomy_tag_name).val(classes + "," + class_title);
                }
              }
              else {
                // Remove the class
                $(this).siblings("span").children(".state").attr('src', taxonomy_not_selected_image);

                var class_title = $(this).val();

                // Get the class ID
                var re = new RegExp("^([0-9\.]+).*$");
                var classid = class_title.replace(re, "$1");

                var classes = $(taxonomy_tag_name).val();

                // Start
                var re = new RegExp("^[ ]*" + classid + " [^\,]+\,? *");
                classes = classes.replace(re, "");

                // Middle
                var re = new RegExp(" *, *" + classid + " [^\,]+\, *");
                classes = classes.replace(re, ",");

                // End
                var re = new RegExp(" *, *" + classid + " .+$");
                classes = classes.replace(re, "");

                $(taxonomy_tag_name).val(jQuery.trim(classes));
              }
            });

            $('.taxon-state').click(function() {
              if ($(this).children(".state").attr('src') != taxonomy_selected_image) {
                // Add the class
                $(this).children(".state").attr('src', taxonomy_selected_image);
                var classid = $(this).siblings("input").val();

                var classes = $(taxonomy_tag_name).val();

                if (classes == "") {
                  $(taxonomy_tag_name).val(classid);
                }
                else {
                  $(taxonomy_tag_name).val(classes + "," + classid);
                }
              }
              else {
                // Remove the class
                $(this).children(".state").attr('src', taxonomy_not_selected_image);

                var class_title = $(this).siblings("input").val();

                // Get the class ID
                var re = new RegExp("^([0-9\.]+).*$");
                var classid = class_title.replace(re, "$1");

                var classes = $(taxonomy_tag_name).val();

                // Start
                var re = new RegExp("^[ ]*" + classid + " [^\,]+\,? *");
                classes = classes.replace(re, "");

                // Middle
                var re = new RegExp(" *, *" + classid + " [^\,]+\, *");
                classes = classes.replace(re, ",");

                // End
                var re = new RegExp(" *, *" + classid + " .+$");
                classes = classes.replace(re, "");

                $(taxonomy_tag_name).val(classes);
              }
            });

            $('.taxon-help').click(function() {
              var id = $(this).attr('id');
              var position = $(this).position();

              // Display the box a little to the right of the image
              var width = $(this).width() + 30;
              var id_text = $(this).siblings("input").attr('value');

              id = id.replace(/\./g, "-");

              var url = location.protocol + "//" + location.host + taxonomy_help_texts + "/" + id + ".html";

              // Get the text for the dialog from the server
              $.ajax({
                type: "GET",
                url: url,
                success: function(data, textStatus){
                  var height = 100;

                  if (data.length > 500)
                  {
                    height = 200;
                  }

                  if (data.length > 1000)
                  {
                    height = 400;
                  }

                  // Remove all open dialogs
                  $(".taxon-help-dialog").remove();

                  // Add a div with the text
                  $("#" + id).append("<div id = 'dialog-" + id + "' class = 'taxon-help-dialog'><div class = 'taxon-dialog-top'><span id = 'taxon-dialog-close-" + id + "' class = 'taxon-dialog-close' title = 'Close'>X</span></div><h3>" + id_text + "</h3><div class = 'taxon-dialog-body'>" + data + "</div></div>");

                  $("#dialog-" + id).css({
                    "top": position.top + "px",
                    "left": (position.left + width) + "px"
                  });

                  $("#dialog-" + id + " .taxon-dialog-body").css({
                    "height": height + "px",
                  });
                },
                error: function(errorObj, textStatus, errorThrown){
                  alert("No help text was found.");
                }
              });
            });

            $('.taxon-dialog-close').live('click', function() {
              // Remove the dialog
              var id = $(this).attr('id');

              // We want to close the entire dialog
              id = id.replace("taxon-dialog-close-", "dialog-");

              $("#" + id).remove();
            });
          });

          $('div').find('#taxon-classify-button').attr('value', taxonomy_button_text);
          $('#taxon-wait-cursor').remove();
        });

        $(taxonomy_tag_name).change(function () {
          var classids_string = "," + $(taxonomy_tag_name).val() + ",";

          var classids = [];

          var re = /\,\s*([0-9][0-9\.]*(?:[0-9]| ))/g;
          var match = {};

          while (match = re.exec(classids_string)) {
            classids.push(match[1].replace(/^\s+|\s+$/g, ''));
          }

          $("#taxon-results input").each(function () {
            var class_title = $(this).val();

            // Get the class ID
            var re = new RegExp("^([0-9\.]+).*$");
            var classid = class_title.replace(re, "$1");

            if(jQuery.inArray(classid, classids) != -1) {
              $(this).siblings("span").children(".state").attr('src', taxonomy_selected_image);
            }
            else {
                $(this).siblings("span").children(".state").attr('src', taxonomy_not_selected_image);
            }
          });
        });
      }
    }
  };

  function getTaxonResult(taxonomy_name, taxonomy_tag_name, taxonomy_text_field, taxonomy_text_field_ckeditor, taxonomy_button_text, taxonomy_button_text_wait, taxonomy_waiticon_image, taxonomy_delay_taxon_call, taxonomy_text_title, taxonomy_text_breadcrumb_text) {
    var taxonomy_contents = $(document).find(taxonomy_tag_name).val();

    if (taxonomy_contents == "") {
      $('div').find('#taxon-classify-button').attr('value', taxonomy_button_text_wait);
      $('div').find('#taxon-classify-button').after("<img id = 'taxon-wait-cursor' src = '" + taxonomy_waiticon_image + "'>");
      $("#taxon-results").remove();

      var text = "";

      if (taxonomy_text_title != "") {
        text += $(taxonomy_text_title).val() + " ";
      }

      if (taxonomy_text_breadcrumb_text != "") {
        // Note that taxonomy_text_breadcrumb_text is the actual text to include
        text = taxonomy_text_breadcrumb_text + " ";
      }

      if (taxonomy_text_field_ckeditor) {
        // Remove the starting #
        taxonomy_text_field_name = taxonomy_text_field.replace(/^\#/, '');

        CKEDITOR.config.entities = false;
        CKEDITOR.config.entities_latin = false;

        try {
          text += CKEDITOR.instances[taxonomy_text_field_name].getData();
        }
        catch(e) {
          alert("Warning from taxon_taxonomy: The field '" + taxonomy_text_field_name + "' does not exist. \nThe text from the CKEditor was not included.\nPlease check the settings for taxon_taxonomy.");
        }
      }
      else {
        text += $(taxonomy_text_field).val();
      }

      $.ajaxSetup({async:false});

      // Post to the Drupal proxy function and get the result back as text
      $.post("/taxon-taxonomy", { taxonomy: taxonomy_name, text: text }, function(data) {
        data = data.replace(/\n/g, ",");
        $(taxonomy_tag_name).val(data);

        $('div').find('#taxon-classify-button').attr('value', taxonomy_button_text);
        $('#taxon-wait-cursor').remove();
      });
    }
  }
})(jQuery);
