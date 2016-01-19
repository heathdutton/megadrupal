(function ($) {

  if (window && window.TraceKit && window.TraceKit.report && window.TraceKit.report.subscribe) {
    window.TraceKit.report.subscribe(function(errorReport) {
      $.post('/tracekit/collect', {
        message: errorReport.message,
        mode: errorReport.mode,
        name: errorReport.name,
        url: errorReport.url,
        useragent: errorReport.useragent,
        stack: JSON.stringify(errorReport.stack)
      });
    });
  }

})(jQuery);
