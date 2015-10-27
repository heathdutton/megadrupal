(function ($) {
Drupal.behaviors.powertagging_field = {
  attach: function (context, settings) {

    $("div.field-type-powertagging").once(function() {
      $(this).find("input[type=\"submit\"]").click(function(event) {
        event.preventDefault();
        var field_id = $(this).parent().parent().parent().attr("id").substr(5).replace(/-/g, "_");
        var html_id = "#edit-" + field_id.replace(/_/g, "-");
        var data = collect_content(Drupal.settings.powertagging[field_id][Object.keys(Drupal.settings.powertagging[field_id])[0]]);
        loading_extracted_tags(html_id);

        $.post(Drupal.settings.basePath + "powertagging/extract", data, function (tags) {
          renderResult(field_id, tags);
        }, "json");
      });
    });

    $(document).ready(function () {
      Object.getOwnPropertyNames(Drupal.settings.powertagging).forEach(function(field_id) {
        renderResult(field_id, {});
      });
    });

    /**
     * Collect all the data into an object for the extraction
     */
    function collect_content (pt_field) {
      var field_id = "#edit-" + pt_field.settings.field_name.replace(/_/g, "-");

      var data = {settings: pt_field.settings, content: "", files: []};
      // Build the text content to extract tags from.
      $.each(pt_field.fields, function(field_index, field) {
        switch (field.module) {
          case "text":
            var text_content = collect_content_text(field.field_name, field.widget) || "";
            if (text_content.length > 0) {
              data.content += (data.content.length > 0 ? " " : "") + text_content;
            }
            break;

          case "file":
            data.files = data.files.concat(collect_content_file(field.field_name, field.widget));
            break;
        }
      });
      return data;
    }

    /**
     * Collect the data from different text field types
     */
    function collect_content_text (field, widget) {
      var field_id = "#edit-" + field.replace(/_/g, "-");
      switch (widget) {
        case "text_textfield_title":
          return $(field_id).val();
          break;

        case "text_textfield":
          return $(field_id + " input").val();
          break;

        case "text_textarea":
        case "text_textarea_with_summary":
          var content = "";
          // Get the text from the summary and the full text.
          $(field_id + " textarea").each(function() {
            var textarea_id = $(this).attr("id");
            // CkEditor.
            if (typeof(CKEDITOR) != "undefined" && CKEDITOR.hasOwnProperty("instances") && CKEDITOR.instances.hasOwnProperty(textarea_id)) {
              content += CKEDITOR.instances[textarea_id].getData();
            }
            // TinyMCE.
            else if (typeof(tinyMCE) != "undefined" && tinyMCE.hasOwnProperty("editors") && tinyMCE.editors.hasOwnProperty(textarea_id)) {
              content += tinyMCE.editors[textarea_id].getContent({format : "raw"});
            }
            // No text editor or an unsupported one.
            else {
              content += $(this).val();
            }
          });
          return content;
          break;
      }
      return "";
    }

    /**
     * Collect the data from different file field types
     */
    function collect_content_file (field, widget) {
      var field_id = "#edit-" + field.replace(/_/g, "-");
      switch (widget) {
        case "file_generic":
          var files = [];
          $(field_id + " table input[type=hidden]").each(function() {
            if ($(this).attr("name").indexOf("[fid]") > 0) {
              files.push($(this).val());
            }
          });
          return files;
          break;
      }
      return [];
    }

    /**
     * Render the PowerTagging data
     */
    function renderResult (field_id, tags) {
      var settings = Drupal.settings.powertagging[field_id][Object.keys(Drupal.settings.powertagging[field_id])[0]];
      var field_id = "#edit-" + field_id.replace(/_/g, "-");
      var tags_extracted = false;

      check_entity_language(field_id, settings.settings);

      // Close the extracted tags container on the beginning
      if (!tags.hasOwnProperty("suggestion")) {
        hide_extracted_tags(field_id);
      }

      if (!tags.hasOwnProperty('messages') || tags.messages.length == 0) {
        // Render the content tags.
        if (tags.hasOwnProperty('content')) {
          var content_tags = tags.content.concepts.concat(tags.content.freeterms);
          if (content_tags.length > 0) {
            var html_tags = [];

            content_tags.forEach(function(tag) {
              html_tags.push(renderTag(tag));
            });

            $(field_id + " #powertagging-extracted-tags").append('<div id="powertagging-extracted-tags-area-content" class="powertagging-extracted-tags-area"><div class="powertagging-extraction-label">' + Drupal.t("Form fields") + "</div><ul><li>" + html_tags.join("</li><li>") + "</li></ul></div>");
            show_extracted_tags(field_id);
          }
        }

        // Render the file tags.
        if (tags.hasOwnProperty('files')) {
          for(var file_name in tags.files) {
            var file_tags = tags.files[file_name].concepts.concat(tags.files[file_name].freeterms);
            if (file_tags.length > 0) {
              var html_tags = [];

              file_tags.forEach(function(tag) {
                html_tags.push(renderTag(tag));
              });

              $(field_id + " #powertagging-extracted-tags").append('<div class="powertagging-extracted-tags-area"><div class="powertagging-extraction-label">' + Drupal.t('Uploaded file "%file"', {'%file': file_name}) + "</div><ul><li>" + html_tags.join("</li><li>") + "</li></ul></div>");
              show_extracted_tags(field_id);
            }
          }
        }

        // Initially add the tags for the results area.
        var result_tags = [];

        // There are already tags connected with this node.
        if (settings.hasOwnProperty("selected_tags") && settings.selected_tags.length > 0) {
          settings.selected_tags.forEach(function(tag) {
            result_tags.push(tag);
          });
        }
        // No tags connected with this node yet, use the suggestion.
        else if (tags.hasOwnProperty("suggestion")) {
          var suggestion_tags = tags.suggestion.concepts.concat(tags.suggestion.freeterms);
          if (suggestion_tags.length > 0) {
            suggestion_tags.forEach(function(tag) {
              result_tags.push(tag);
            });
          }
        }

        // Add the tags to the result.
        result_tags.forEach(function(result_tag) {
          addTagToResult(field_id, result_tag);
        });
      }
      // There are errors or infos available --> show them instead of the tags.
      else {
        var messages_html = '';
        tags.messages.forEach(function(message){
          messages_html += '<div class="messages ' + message.type + '">' + message.message + '</div>';
        });
        $(field_id + " #powertagging-extracted-tags").html(messages_html);
        show_extracted_tags(field_id);
      }

      // Add the click handlers to the tag-elements.
      $(field_id + " #powertagging-extracted-tags .powertagging-tag").click(function() {
        if ($(this).hasClass('disabled')) {
          removeTagFromResult(field_id, tagElementToObject($(this)));
        }
        else {
          addTagToResult(field_id, tagElementToObject($(this)));
        }
      });
      $(field_id + " #powertagging-tag-result .powertagging-tag").click(function() {
        removeTagFromResult(field_id, tagElementToObject($(this)));
      });

      // Manually add an existing concept or freeterm to the result.
      $(field_id + " #powertagging-tag-result").closest(".field-type-powertagging").find("input.powertagging_autocomplete_tags").autocomplete({
        source: Drupal.settings.basePath + "powertagging/autocomplete_tags/" + settings.settings.powertagging_id + '/' + settings.settings.project_id + '/' + settings.settings.entity_language,
        minLength: 2,
        select: function( event, ui ) {
          event.preventDefault();
          addTagToResult(field_id, {tid: ui.item.tid, uri: ui.item.uri, label: ui.item.label, type: ui.item.type});
          $(this).val('');
        }
      })
      // Manually add a new freeterm to the result.
      .keyup(function(e){
        if (e.keyCode == 13) {
          var field_value = jQuery.trim($(this).val());
          if (field_value.length > 0) {
            addTagToResult(field_id, {tid: 0, uri: '', label: field_value, type: 'freeterm'});
            $(this).autocomplete("close");
            $(this).val("");
          }
        }
      });

      // Update the language of the entity.
      if ($(field_id).closest('form').find('#edit-language').length > 0) {
        $(field_id).closest('form').find('#edit-language').change(function() {
          // Language selection as a select-element.
          if ($(this).is('select')) {
            var language = $(this).val();
          }
          // Language selection as radio buttons.
          else if ($(this).find('input:checked').length > 0) {
            var language = $(this).find('input:checked').val();
          }

          Object.getOwnPropertyNames(Drupal.settings.powertagging).forEach(function(field_id) {
            Drupal.settings.powertagging[field_id][Object.keys(Drupal.settings.powertagging[field_id])[0]].settings.entity_language = language;
            var html_field_id = "#edit-" + field_id.replace(/_/g, '-');
            var field_settings = Drupal.settings.powertagging[field_id][Object.keys(Drupal.settings.powertagging[field_id])[0]].settings;
            check_entity_language(html_field_id, field_settings);
            
            // Update the the autocomplete path.
            $(html_field_id + " #powertagging-tag-result").closest(".field-type-powertagging").find("input.powertagging_autocomplete_tags").autocomplete(
              'option', 'source', Drupal.settings.basePath + "powertagging/autocomplete_tags/" + settings.settings.powertagging_id + '/' + settings.settings.project_id + '/' + settings.settings.entity_language
            );
          });
        });
      }
    }

    /**
     * Create a JS-object out of a jQuery element for a powertagging tag.
     */
    function tagElementToObject(tag_object) {
      var type = tag_object.hasClass('concept') ? 'concept' : 'freeterm';
      return {tid: tag_object.attr("data-tid"), uri: tag_object.attr("data-uri"), label: tag_object.attr("data-label"), type: type};
    }

    /**
     * Get the HTML-code of a single powertagging tag.
     *
     * @param object tag
     *   The tag with properties "tid" and "label"
     *
     * @return string
     *   The HTML-output of the tag
     */
    function renderTag(tag) {
      var score = tag.score ? ' (' + tag.score + ')' : '';
      return '<div class="powertagging-tag ' + tag.type + '" data-tid="' + tag.tid + '" data-label="' + tag.label + '" data-uri="' + tag.uri + '">' + tag.label + score + '</div>';
    };

    function addTagToResult(field_id, tag) {
      // Only add tags, that are not already inside the results area.
      if ((tag.tid > 0 && $(field_id + ' #powertagging-tag-result .powertagging-tag[data-tid="' + tag.tid + '"]').length == 0) ||
          (tag.tid == 0 && ($(field_id + ' #powertagging-tag-result .powertagging-tag[data-label="' + tag.label + '"]').length == 0 || tag.uri != ""))) {

        // Add a new list if this is the first tag to add.
        if ($(field_id + " #powertagging-tag-result ul").length == 0) {
          $(field_id + " #powertagging-tag-result").append("<ul></ul>");
        }

        // Remove freeterms with the same string for concepts.
        if (tag.type == 'concept') {
          $(field_id + ' #powertagging-tag-result .powertagging-tag[data-label="' + tag.label + '"]').parent("li").remove();
        }

        // Add a new list item to the result.
        $(field_id + " #powertagging-tag-result ul").append("<li>" + renderTag(tag) + "</li>");

        // Add a click handler to the new result tag.
        $(field_id + " #powertagging-tag-result li:last-child .powertagging-tag").click(function() {
          removeTagFromResult(field_id, tagElementToObject($(this)));
        });

        // Update the field value to save.
        updateFieldValue(field_id);
      }

      // Disable already selected tags in the extraction area.
      if (tag.tid > 0) {
        $(field_id + ' #powertagging-extracted-tags .powertagging-tag[data-tid="' + tag.tid + '"]').addClass("disabled");
      }
      else {
        $(field_id + ' #powertagging-extracted-tags .powertagging-tag[data-label="' + tag.label + '"]').addClass("disabled");
      }
    }

    function removeTagFromResult(field_id, tag) {
      var settings = Drupal.settings.powertagging[field_id.replace('#edit-', '').replace(/-/g, '_')];
      settings = settings[Object.keys(settings)[0]];

      // Enable tags in the extraction area again and remove the list item.
      if (tag.tid > 0) {
        $(field_id + ' #powertagging-tag-result .powertagging-tag[data-tid="' + tag.tid + '"]').parent("li").remove();
        $(field_id + ' #powertagging-extracted-tags .powertagging-tag[data-tid="' + tag.tid + '"]').removeClass("disabled");
      }
      else {
        $(field_id + ' #powertagging-tag-result .powertagging-tag[data-label="' + tag.label + '"]').parent("li").remove();
        $(field_id + ' #powertagging-extracted-tags .powertagging-tag[data-label="' + tag.label + '"]').removeClass("disabled");
      }

      // No empty ULs are allowed, remove them.
      if ($(field_id + " #powertagging-tag-result li").length == 0) {
        $(field_id + " #powertagging-tag-result ul").remove();
      }

      // Also remove the tag from the selected tags in the Drupal settings.
      if (settings.hasOwnProperty("selected_tags") && settings.selected_tags.length > 0) {
        for (var tag_index = 0; tag_index < settings.selected_tags.length; tag_index++) {
          if (settings.selected_tags[tag_index].label, tag.label) {
            settings.selected_tags.splice(tag_index, 1);
            break;
          }
        }
      }

      // Update the field value to save.
      updateFieldValue(field_id);
    }

    function updateFieldValue(field_id) {
      var tags_to_save = [];
      // Use tid for existing terms and label for new free terms.
      $(field_id + " #powertagging-tag-result .powertagging-tag").each(function() {
        if ($(this).attr("data-tid") > 0) {
          tags_to_save.push($(this).attr("data-tid"));
        }
        else {
          tags_to_save.push($(this).attr("data-label").replace(',', ';') + '|' + $(this).attr("data-uri"));
        }
      });

      $(field_id + " #powertagging-tag-result").closest(".field-type-powertagging").find("input.powertagging_tag_string").val(tags_to_save.join(','));
    }

    /**
     * Check if the currently selected entity language is allowed
     *
     * @param string field_id
     *   The ID of the DOM element of the powertagging field
     * @param array settings
     *   An array of settings of the powertagging field
     */
    function check_entity_language(field_id, settings) {
      var language_error_element = $('#' + $(field_id).children('fieldset').attr('id') + '-language-error');
      // The currently selected entity language is allowed.
      if ($.inArray(settings.entity_language, settings.allowed_languages) > -1) {
        language_error_element.hide();
      }
      // The currently selected entity language is not allowed.
      else {
        language_error_element.show();
      }
    }

    /**
     * Show loading image
     *
     * @param string field_id
     *   The ID of the DOM element of the powertagging field
     */
    function loading_extracted_tags(field_id) {
      // Clear the extracted tags area
      $(field_id + " #powertagging-extracted-tags").slideUp().html("");
      $(field_id + " #powertagging-extracted-tags").prev().slideDown();
      $(field_id + " #powertagging-extracted-tags").parent().slideDown();
    }

    /**
     * Show extracted tags
     *
     * @param string field_id
     *   The ID of the DOM element of the powertagging field
     */
    function show_extracted_tags(field_id) {
      $(field_id + " #powertagging-extracted-tags").show();
      $(field_id + " #powertagging-extracted-tags").prev().slideUp();
      $(field_id + " #powertagging-extracted-tags").slideDown();
    }

    /**
     * Hide extracted tags
     *
     * @param string field_id
     *   The ID of the DOM element of the powertagging field
     */
    function hide_extracted_tags(field_id) {
      $(field_id + " #powertagging-extracted-tags").parent().hide();
      $(field_id + " #powertagging-extracted-tags").prev().hide();
      $(field_id + " #powertagging-extracted-tags").hide();
    }

  }
}
})(jQuery);
