(function($) {

  Drupal.behaviors.contextlyNodeFormVerticalTab = {
    attach: function(context, settings) {
      $('fieldset#edit-contextly', context)
        .once('contextly-drupal-summary')
        .drupalSetSummary(function(context) {
          var vals = [];

          var disabled = $('input[name="contextly_disabled"]', context).is(':checked');
          if (disabled) {
            vals.push(Drupal.t('Disabled'));
          }
          else {
            vals.push(Drupal.t('Enabled'));
          }

          return vals.join(', ');
        });
    }
  };

  Drupal.behaviors.contextlyNodeFormSnippetEdit = {
    containers: $([]),

    addContainers: function(containers) {
      containers.show();
      this.containers = this.containers.add(containers);

      // Apply buttons state depending on editor state.
      if (Contextly.editor.isLoaded) {
        this.onSettingsLoaded();
      }
      else if (Contextly.editor.hasFailed) {
        this.onSettingsFailed();
      }
      else if (Contextly.editor.isLoading) {
        this.onSettingsLoading();
      }
    },

    getButtons: function() {
      return this.containers.find('.d-ctx-snippet-button');
    },

    getDescriptions: function() {
      return this.containers.find('.d-ctx-snippet-status');
    },

    getButtonLabel: function() {
      var hasLinks = Drupal.settings.contextlyEditor
        && Drupal.settings.contextlyEditor.snippet
        && Drupal.settings.contextlyEditor.snippet.links;
      if (hasLinks) {
        return Drupal.t('Edit Related Posts');
      }
      else {
        return Drupal.t('Choose Related Posts');
      }
    },

    onSettingsLoaded: function () {
      this.getButtons()
        .attr('value', this.getButtonLabel())
        .unbind('click.contextly')
        .bind('click.contextly', function (e) {
          e.preventDefault();
          Contextly.editor.open('snippet');
        });
      this.getDescriptions()
        .empty()
        .hide();
    },

    onSettingsFailed: function () {
      this.getButtons()
        .attr('value', Drupal.t('Try again'))
        .unbind('click.contextly')
        .bind('click.contextly', function (e) {
          e.preventDefault();
          Contextly.editor.loadSettings();
        });
      this.getDescriptions()
        .empty()
        .text(Drupal.t('Loading of the Contextly settings has failed. Click "Try again" button to make another attempt.'))
        .show();
    },

    onSettingsLoading: function () {
      this.getButtons()
        .attr('value', Drupal.t('Loading...'))
        .unbind('click.contextly')
        .bind('click.contextly', function (e) {
          e.preventDefault();
        });
      this.getDescriptions()
        .empty()
        .hide();
    },

    onWidgetUpdated: function() {
      this.getButtons()
        .attr('value', this.getButtonLabel());
    },

    attach: function (context, settings) {
      if (!window.Contextly || !Contextly.editor) {
        return;
      }

      $('body', context).once('d-ctx-snippet-edit', this.proxy(function() {
        $(window).bind({
          contextlySettingsLoading: this.proxy(this.onSettingsLoading),
          contextlySettingsLoaded: this.proxy(this.onSettingsLoaded),
          contextlySettingsFailed: this.proxy(this.onSettingsFailed),
          contextlyWidgetUpdated: this.proxy(this.onWidgetUpdated),
          contextlyWidgetRemoved: this.proxy(this.onWidgetUpdated)
        });
      }));

      var containers = $('.d-ctx-snippet-edit', context)
        .once('d-ctx-snippet-edit');
      if (containers.length) {
        this.addContainers(containers);
      }
    },

    detach: function(context) {
      // Remove the links found in the context from the global list of links.
      var contextContainers = $('.d-ctx-snippet-edit', context);
      this.containers = this.containers.not(contextContainers);
    },

    proxy: function(callback) {
      return $.proxy(callback, this);
    }
  };

})(jQuery);
