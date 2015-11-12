(function ($) {
  Drupal.behaviors.flexpaper = {
    attach: function (context, settings) {
      //Get paths for swf and json files
      var swfFiles = Drupal.settings.flexpaper.swfFiles;
      var jsonFiles = Drupal.settings.flexpaper.jsonFiles;
      var pdfFiles = Drupal.settings.flexpaper.pdfFiles;
      var pngFiles = Drupal.settings.flexpaper.pngFiles;
      var renderOrder = Drupal.settings.flexpaper.renderOrder;
      var version = Drupal.settings.flexpaper.version;
      var licenseKey = Drupal.settings.flexpaper.licenseKey;

      var flexpaper_init = function (element_id, index) {
        $('#' + element_id).FlexPaperViewer(
            { config: {
              SwfFile: swfFiles[index],
              JSONFile: jsonFiles[index],
              IMGFiles: pngFiles[index],
              PDFFile: pdfFiles[index],

              Scale: Drupal.settings.flexpaper.scale,
              ZoomTransition: Drupal.settings.flexpaper.zoomTransition,
              ZoomTime: Drupal.settings.flexpaper.zoomTime,
              ZoomInterval: Drupal.settings.flexpaper.zoomInterval,
              FitPageOnLoad: Drupal.settings.flexpaper.fitPageOnLoad == 1,
              FitWidthOnLoad: Drupal.settings.flexpaper.fitWidthOnLoad == 1,
              FullScreenAsMaxWindow: Drupal.settings.flexpaper.fullScreenAsMaxWindow == 1,
              ProgressiveLoading: Drupal.settings.flexpaper.progressiveLoading == 1,
              MinZoomSize: Drupal.settings.flexpaper.minZoomSize,
              MaxZoomSize: Drupal.settings.flexpaper.maxZoomSize,
              SearchMatchAll: Drupal.settings.flexpaper.searchMatchAll == 1,
              InitViewMode: Drupal.settings.flexpaper.initViewMode,
              RenderingOrder: renderOrder,

              jsDirectory: Drupal.settings.flexpaper.jsDirectory,
              cssDirectory: Drupal.settings.flexpaper.cssDirectory,
              localeDirectory: Drupal.settings.flexpaper.localeDirectory,

              key: Drupal.settings.flexpaper.flexpaperKey,
              ViewModeToolsVisible: Drupal.settings.flexpaper.viewModeToolsVisible == 1,
              ZoomToolsVisible: Drupal.settings.flexpaper.zoomToolsVisible == 1,
              NavToolsVisible: Drupal.settings.flexpaper.navToolsVisible == 1,
              CursorToolsVisible: Drupal.settings.flexpaper.cursorToolsVisible == 1,
              SearchToolsVisible: Drupal.settings.flexpaper.searchToolsVisible == 1,
              localeChain: 'en_US',

              JSONDataType: 'json'
            }}
        );
      };

      var placeHolders = $('div.flexpaper_viewer');

      //Put ids for this elements, because flexpaper need it.
      placeHolders.each(function (index) {
        var elementId = 'documentViewer_' + index;
        $(this).attr('id', elementId);
        flexpaper_init(elementId, index);
      });
    }
  }
})(jQuery);
