/**
 * @file
 * Create OpenTags UI.
 *
 * This will replace the original Metatag controls with the OpenTags widgets.
 */

(function($) {

  /**
   * Create OpenTags Widgets on the page.
   *
   * @param string id
   *   The id attribute value assigned to the original html control element.
   * @param string widgetType
   *   The type of the widget: 'autocombo' or 'dropdown'.
   * @param object settings
   *   A number of settings used to create the widget.
   *   Properties:
   *   - vocabName: vocabulary name.
   *   - widgetName: widget name displayed as label.
   *   - widgetID: widget ID.
   *   - srcControlClass: the class name of the source metatag element.
   *   - delimiter: delimiter used to concatenate multiple values.
   */
  var createOpenTagsWidget = function(id, widgetType, settings) {
    switch (widgetType) {
      case 'autocombo':
        $('.opentags-container .fieldset-wrapper').append('<input type="text" id="' + id + '"><br />');
        break;

      case 'dropdown':
        $('.opentags-container .fieldset-wrapper').append('<select id="' + id + '"><option></option></select><br />');
        break;
    }

    $('#' + id).after('<div>Loading...</div>');
    $('#' + id).hide();
    $.ajax({
      method: "POST",
      async: true,
      url: Drupal.settings.basePath + "opentags/services/get-terms-data",
      data: {vocab: settings.vocabName},
      dataType: 'json',
      success: function(data) {
        $('#' + id).show();
        $('#' + id).next().remove();
        switch (widgetType) {
          case 'autocombo':
            var autocomboSettings = {
              name: settings.widgetName,
              id: settings.widgetID,
              items: data['data']
            };

            var delimiter = settings.delimiter ? settings.delimiter : ";";
            autocomboSettings['delimiter'] = delimiter;

            var default_values = getWidgetDefaultValue(settings.srcControlClass, delimiter);
            if (default_values) {
              autocomboSettings['default'] = default_values;
            }

            $('#' + id).openTagsAutoCombo('init', autocomboSettings);

            // Bind click event.
            $('#edit-submit, #edit-save').click(function(e) {
              $('.' + settings.srcControlClass).val($('#' + id).openTagsAutoCombo('value'));
            });
            break;

          case 'dropdown':
            var dropdownSettings = {
              name: settings.widgetName,
              id: settings.widgetID,
              items: data['data']
            };

            var default_value = getWidgetDefaultValue(settings.srcControlClass, null);
            if (default_value) {
              dropdownSettings['default'] = default_value;
            }

            $('#' + id).openTagsDropdown('init', dropdownSettings);

            // Bind click event.
            $('#edit-submit, #edit-save').click(function(e) {
              $('.' + settings.srcControlClass).val($('#' + id).openTagsDropdown('value'));
            });
            break;
        }

      }
    });
  };

  var getWidgetDefaultValue = function(srcControlClass, delimiter) {
    var rawValue = $('.' + srcControlClass).val();
    if (rawValue && rawValue != '') {
      return rawValue.split(delimiter);
    }
  };

  $('.opentags-src-field-hidden').parent().hide();
  $('.opentags-container .token-dialog').hide();

  var widgets = [
    {
      id: 'ot-audience',
      type: 'autocombo',
      settings: {
        vocabName: 'opentags_audience',
        widgetName: 'Audience',
        widgetID: 'audience',
        srcControlClass: 'opentags-src-audience'
      }
    },
    {
      id: 'ot-coverage',
      type: 'autocombo',
      settings: {
        vocabName: 'opentags_coverage',
        widgetName: 'Coverage',
        widgetID: 'coverage',
        srcControlClass: 'opentags-src-coverage'
      }
    },
    {
      id: 'ot-function',
      type: 'autocombo',
      settings: {
        vocabName: 'opentags_function',
        widgetName: 'Function',
        widgetID: 'function',
        srcControlClass: 'opentags-src-function'
      }
    },
    {
      id: 'ot-language',
      type: 'autocombo',
      settings: {
        vocabName: 'opentags_language',
        widgetName: 'Language',
        widgetID: 'language',
        srcControlClass: 'opentags-src-language'
      }
    },
    {
      id: 'ot-type',
      type: 'dropdown',
      settings: {
        vocabName: 'opentags_type',
        widgetName: 'Type',
        widgetID: 'type',
        srcControlClass: 'opentags-src-type'
      }
    },
    {
      id: 'ot-subject',
      type: 'autocombo',
      settings: {
        vocabName: 'opentags_subject',
        widgetName: 'Subject',
        widgetID: 'subject',
        srcControlClass: 'opentags-src-subject'
      }
    }
  ];

  for (var i = 0; i < widgets.length; i++) {
    createOpenTagsWidget(widgets[i].id, widgets[i].type, widgets[i].settings);
  }

}(jQuery));
