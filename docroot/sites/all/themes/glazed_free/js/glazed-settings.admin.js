(function ($, Drupal) {
  /*global jQuery:false */
  /*global Drupal:false */
  "use strict";

  /**
   * Provide vertical tab summaries for Bootstrap settings.
   */
  Drupal.behaviors.glazedSettingsControls = {
    attach: function (context) {
      var $context = $(context);

      // Convert checkboxes to switches
      $.fn.bootstrapSwitch.defaults.onColor = "success";
      $.fn.bootstrapSwitch.defaults.onText = "Yes";
      $.fn.bootstrapSwitch.defaults.offText = "No";
      $.fn.bootstrapSwitch.defaults.size = "small";
      $.fn.bootstrapSwitch.defaults.onSwitchChange = function(event, state) { setTimeout(function(){ $('.slider + input').bootstrapSlider('relayout'); }, 10); };
      $("[type='checkbox']").bootstrapSwitch();
      // This patched up incompatibility with $ <1.10
      // https://github.com/nostalgiaz/bootstrap-switch/issues/474
      $("[type='checkbox']").on('switchChange.bootstrapSwitch', function(event, state) {
        $(this).trigger('change');
      });

      // Opacity sliders
      var $opacitySliders = $('#edit-header-top-bg-opacity-scroll, #edit-header-top-bg-opacity, #edit-header-side-bg-opacity, #edit-side-header-background-opacity,#edit-page-title-image-opacity,#edit-header-top-opacity,#edit-header-top-opacity-scroll,#edit-menu-full-screen-opacity');
      var startValue = 1;
      $opacitySliders.each( function() {
        startValue = $(this).val();
        $(this).bootstrapSlider({
          step: 0.01,
          min: 0,
          max: 1,
          tooltip: 'always',
          value: parseFloat(startValue)
        });
      });

      // Page Title height
      var $pageTitleHeight = $('#edit-page-title-height');
      $pageTitleHeight.bootstrapSlider({
        step: 5,
        min: 50,
        max: 500,
        tooltip: 'always',
        formatter: function(value) {return value + ' px';},
        value: parseFloat($pageTitleHeight.val())
      });

      // Header height slider
      var $headerHeight = $('#edit-header-top-height');
      $headerHeight.bootstrapSlider({
        step: 1,
        min: 10,
        max: 200,
        tooltip: 'always',
        formatter: function(value) {return value + ' px';},
        value: parseFloat($headerHeight.val())
      });

      // Header Mobile height slider
      var $headerHeight = $('#edit-header-mobile-height');
      $headerHeight.bootstrapSlider({
        step: 1,
        min: 10,
        max: 200,
        tooltip: 'always',
        formatter: function(value) {return value + ' px';},
        value: parseFloat($headerHeight.val())
      });

      // Header after-scroll height slider
      var $headerScrollHeight = $('#edit-header-top-height-scroll');
      $headerScrollHeight.bootstrapSlider({
        step: 1,
        min: 10,
        max: 200,
        tooltip: 'always',
        formatter: function(value) {return value + ' px';},
        value: parseFloat($headerScrollHeight.val())
      });

      // Sticky header scroll offset
      var $headerScrollOffset = $('#edit-header-top-height-sticky-offset');
      $headerScrollOffset.bootstrapSlider({
        step: 10,
        min: 0,
        max: 2096,
        tooltip: 'always',
        formatter: function(value) {return value + ' px';},
        value: parseFloat($headerScrollOffset.val())
      });

      // Side Header after-scroll height slider
      var $headerHeight = $('#edit-header-side-width');
      $headerHeight.bootstrapSlider({
        step: 5,
        min: 50,
        max: 500,
        tooltip: 'always',
        formatter: function(value) {return value + ' px';},
        value: parseFloat($headerHeight.val())
      });

      // Layout max width
      var $width = $('#edit-layout-max-width');
      $width.bootstrapSlider({
        step: 10,
        min: 480,
        max: 2560,
        tooltip: 'always',
        formatter: function(value) {return value + ' px';},
        value: parseFloat($width.val())
      });

      // Layout Gutter Horizontal
      var $gutterHor = $('#edit-gutter-horizontal');
      $gutterHor.bootstrapSlider({
        step: 1,
        min: 0,
        max: 100,
        tooltip: 'always',
        formatter: function(value) {return value + ' px';},
        value: parseFloat($gutterHor.val())
      });

      // Layout Gutter Vertical
      var $gutterVer = $('#edit-gutter-vertical');
      $gutterVer.bootstrapSlider({
        step: 1,
        min: 0,
        max: 100,
        tooltip: 'always',
        formatter: function(value) {return value + ' px';},
        value: parseFloat($gutterVer.val())
      });

      // Layout Gutter Horizontal Mobile
      var $gutterHor = $('#edit-gutter-horizontal-mobile');
      $gutterHor.bootstrapSlider({
        step: 1,
        min: 0,
        max: 100,
        tooltip: 'always',
        formatter: function(value) {return value + ' px';},
        value: parseFloat($gutterHor.val())
      });

      // Layout Gutter Vertical Mobile
      var $gutterVer = $('#edit-gutter-vertical-mobile');
      $gutterVer.bootstrapSlider({
        step: 1,
        min: 0,
        max: 100,
        tooltip: 'always',
        formatter: function(value) {return value + ' px';},
        value: parseFloat($gutterVer.val())
      });

      // Reflow layout when showing a tab
      // var $sliders = $('.slider + input');
      // $sliders.each( function() {
      //   $slider = $(this);
      //   $('.vertical-tab-button').click(function() {
      //     $slider.bootstrapSlider('relayout');
      //   });
      // });
      $('.vertical-tab-button a').click(function() {
        $('.slider + input').bootstrapSlider('relayout');
      });
      $('input[type="radio"]').change(function() {
        $('.slider + input').bootstrapSlider('relayout');
      });
    }
  };

  /**
   * Provide vertical tab summaries for Bootstrap settings.
   */
  Drupal.behaviors.glazedSettingSummaries = {
    attach: function (context) {
      var $context = $(context);

      // Page Title.
      $context.find('#edit-page-title').drupalSetSummary(function () {
        return 'Premium Feature';
      });

      // Menu.
      $context.find('#edit-menu').drupalSetSummary(function () {
        var summary = [];

        var menu = $context.find('input[name="menu_type"]:checked');
        if (menu.val()) {
          summary.push(Drupal.t('@menu', {
            '@menu': menu.find('+label').text()
          }));
        }
        return summary.join(', ');

      });

      // Colors.
      $context.find('#color_scheme_form').drupalSetSummary(function () {
        var summary = [];

        var scheme = $context.find('select[name="scheme"] :selected');
        if (scheme.val()) {
          summary.push(Drupal.t('@scheme', {
            '@scheme': scheme.text()
          }));
        }
        return summary.join(', ');

      });

      // Layout.
      $context.find('#edit-layout').drupalSetSummary(function () {
        var summary = [];

        var layoutWidth = $context.find('input[name="layout_max_width"]');
        if (layoutWidth.length) {
          summary.push(Drupal.t('@layoutWidth', {
            '@layoutWidth': layoutWidth.val() + 'px'
          }));
        }

        return summary.join(', ');

      });

      // Hero region.
      $context.find('#edit-hero').drupalSetSummary(function () {
        return 'Premium Feature';
      });

      // Header.
      $context.find('#edit-header').drupalSetSummary(function () {
        return 'Premium Feature';
      });

      // Typography.
      $context.find('#edit-fonts').drupalSetSummary(function () {
        return 'Premium Feature';
      });
    }
  };

})(jQuery, Drupal);