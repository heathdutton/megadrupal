(function($) {
  Highcharts.wrap(Highcharts.Chart.prototype, 'showCredits', function (proceed, credits) {
    proceed.call(this, credits);

    if (this.credits && credits.target) {
      this.credits.on('click', function () {
        window.open(credits.href, credits.target);
      });
    }
  });
})(jQuery);
