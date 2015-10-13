
(function ($) {
  // This module will control the lingwo_fields form.
  var entry = null;

  var field_map = {};
  var timer = null;

  // updates the on screen form with values from the entry
  function updateForm() {
    var name;

    // get rid of old cached values
    entry.clearCache();

    for (name in field_map) {
      var control = field_map[name];
      if (control.automatic) {
        control.fromEntry();
      }
    }
  };

  function setupAddValueLink(name, wrapperNode) {
    var addValueNode = $('<a class="lingwo-fields-addvalue" href="#">'+Drupal.t('Add value')+'</a>'),
        id = $("input[name='lingwo_fields[" + name + "][value]']", wrapperNode).attr('id'),
        label = $("label[for='" + id + "']", wrapperNode);

    addValueNode.click(function () {
      $("input[name='_lingwo_fields[extra_value]']", $(this).closest('form')).val(name);
      $('#edit-lingwo-fields-refresh').click();
      return false;
    });

    // put the addValueNode after the label
    label.append(' ');
    label.append(addValueNode);
  }

  function Control (node) {
    this.type = $(node).attr('data-type'),
    this.name = $(node).attr('data-name');

    this.inputNode = $('.lingwo-fields-value', node).get(0);
    this.wrapperNode = $(node).get(0);
    this.valueNode = $('<a href="#" class="lingwo-fields-value"></a>');
    this.autoNode = $("input[name='lingwo_fields[" + this.name + "][automatic]']", node).get(0);

    if (this.type == 'form') {
      setupAddValueLink(this.name, this.wrapperNode);
    }

    $(this.inputNode).after(this.valueNode);

    this._reattachCheckbox();
    this._attachEvents();
    this.updateAutomatic();
  };
  // does some swanky magic to re-arrange the checkbox for a tighter UI
  Control.prototype._reattachCheckbox = function () {
    // first we want to seperate the label from the check box and hide the label
    $('label', $(this.autoNode).parent()).hide().attr('for', this.autoNode.id);

    // then we want to move the check box to be before the input
    $(this.autoNode).parent().insertBefore($(this.inputNode).parent());
    $(this.autoNode).addClass('lingwo-fields-control-prefix-item');
  };
  // sets up all the proper event handlers to make this control work
  Control.prototype._attachEvents = function () {
    var control   = this,
      type    = this.type,
      name    = this.name,
      inputNode = this.inputNode,
      valueNode = this.valueNode,
      autoNode  = this.autoNode;

    // implement our toggling between automatic/manual
    $(autoNode).bind('click', function (evt) {
      control.updateAutomatic();
      updateForm();
    });
    $(valueNode).bind('click', function (evt) {
      if (control.automatic) {
        $(autoNode).removeAttr('checked');
      }
      else {
        $(autoNode).attr('checked', 'checked');
      }

      // toggle the class value immediately
      if (control.type == 'class' && control.automatic) {
        var value = ($(':selected', inputNode).val() == '1');
        $(control.inputNode).val(value ? '0' : '1');
      }

      control.updateAutomatic();
      return false;
    });

    // attach events for catching changes and updating the form
    $(inputNode).bind(type == 'form' ? 'keyup' : 'change', function (evt) {
      control.toEntry();
      updateForm();
    });
  };
  // takes the value from the autoNode and toggles the controls
  Control.prototype.updateAutomatic = function () {
    this.automatic = $(this.autoNode).attr('checked') ? true : false;
    if (this.automatic) {
      $(this.inputNode).hide();
      $(this.valueNode).show();
      delete entry.fields[this.name];
    }
    else {
      $(this.inputNode).show();
      $(this.valueNode).hide();

      // copy from the form to the entry
      this.toEntry();
      updateForm();
    }
  };
  // pulls the values for this control from an entry
  Control.prototype.fromEntry = function () {
    var value    = '',
      inputValue = '',
      showFunc;

    if (entry.headword) {
      value = inputValue = entry.getField(this.name).toString();
      if (this.type == 'class') {
        inputValue = (value == 'true') ? '1' : '0';
      }
    }

    if (value == '') {
      value = '<i>(empty)</i>';
    }

    $(this.inputNode).val(inputValue);
    $(this.valueNode).html(value);

    // Run our show function if one exists
    if (showFunc = entry.getFieldInfo(this.name).show) {
      if (showFunc(entry)) {
        $(this.wrapperNode).show();
      }
      else {
        $(this.wrapperNode).hide();
      }
    }
  };
  // pulls values from the form and pushs them to the entry
  Control.prototype.toEntry = function () {
    switch(this.type) {
      case 'class':
        entry.fields[this.name] = $(':selected', this.inputNode).val() == '1';
        break;
      case 'option':
        entry.fields[this.name] = $(':selected', this.inputNode).val();
        break;
      case 'form':
        entry.fields[this.name] = $(this.inputNode).val();
        break;
    };
  };

  function setupExtraField (node) {
    var name = $(node).attr('data-name'),
      type = $(node).attr('data-type'),
      inputNode = $('.lingwo-fields-value', node).get(0),
      removeNode = $("input[name='lingwo_fields[" + name + "][remove]']", node).get(0),
      addValueNode, label, removeButton;

    if (type == 'form' && name != '_noname_') {
      setupAddValueLink(name, node);
    }

    // add the remove button
    removeButton = $('<a href="#" class="lingwo-fields-remove-button"></a>');
    removeButton.click(function (evt) {
      $(removeNode).attr('checked', 'checked').click();
      return false;
    });
    removeButton.addClass('lingwo-fields-control-prefix-item');
    removeButton.insertBefore(inputNode.parentNode);
    $(removeNode).parent().hide();
  }

  function setupAddNewForm (node) {
    var nameNode = $('.lingwo-fields-name', this.wrapperNode),
      nonameNode = $('<a class="lingwo-fields-noname" href="#">'+Drupal.t('Set name')+'</a>'),
      legend;

    nonameNode.click(function (evt) {
      nameNode.parent().show();
      nameNode.focus();
      nameNode.get(0).select();
      nonameNode.hide();
      return false;
    });
    nameNode.blur(function (evt) {
      if (nameNode.val() == '') {
        nameNode.val('_noname_').parent().hide();
        nonameNode.show();
      }
    });

    // put the noname node after the legend
    legend = $('legend', node);
    legend.append(' ');
    legend.append(nonameNode);

    nameNode.parent().hide();
  }

  Drupal.behaviors.lingwo_fields = {
    attach: function (context, settings) {
      // get the language/pos from the form
      var lang = $('#edit-language :selected').val() || settings.lingwo_fields.language;
      var pos  = $('#edit-pos :selected').val();

      function triggerAjax() {
        var id = 'edit-lingwo-fields-refresh',
          ajax_settings = settings.ajax[id],
          event_name = ajax_settings['event'],
          selector = ajax_settings['selector'];

        $(selector).trigger(event_name);
      }

      // bind to language and pos to trigger AHAH
      Drupal.lingwo_entry.setCallback('lingwo_fields', 'language', triggerAjax);
      Drupal.lingwo_entry.setCallback('lingwo_fields', 'pos', triggerAjax);

      require(
        ['lingwo/languages/'+lang,
         'lingwo/Entry'],
        function (lang, Entry) {
          // this will be run every time the AHAH completes, so we need to rebuild
          // the entry object.
          entry = new Entry({
            // NOTE: we don't search for #edit-title on context because we want it always
            headword: $('#edit-title').val(),
            language: lang,
            pos: pos
          });

          $('#edit-title', context).once('lingwo_fields', function () {
            $(this).bind('keyup', function (evt) {
              entry.headword = evt.target.value;
              if (timer !== null) {
                clearTimeout(timer);
              }
              timer = setTimeout(function () {
                updateForm();
                timer = null;
              }, 500);
            });
          });

          field_map = {};
          $('.lingwo-fields-control', context).once('lingwo_fields', function () {
            $(this).each(function (i) {
              var hasDefinition = $(this).attr('data-has-definition') == 'true',
                control;

              if (hasDefinition) {
                control = new Control(this);
                field_map[control.name] = control;
              }
              else {
                setupExtraField(this);
              }
            });
          });
          $('#edit--lingwo-fields-add-new-form', context).once('lingwo_fields', function () {
            $(this).each(function (i) {
              setupAddNewForm(this);
            });
          });

          // updated the form!!
          updateForm();
        }
      );
    }
  };
})(jQuery);

