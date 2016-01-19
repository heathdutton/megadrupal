/**
 * @file
 * D3 Sparkline library.
 */

(function ($) {
  Drupal.d3.sparkline = function (select, settings) {
    var w = 100, h = 20,
      data = (settings.rows.map(function (d, index) {
        return {x: new Date(d[0]), y: +d[1]};
      })),
      // Inset by 2 to prevent clipping.
      x = d3.time.scale.utc().domain([data[0].x, data[data.length - 1].x]).range([2, w - 2]),
      y = d3.scale.linear().domain([0, d3.max(data.map(function (d) {
        return d.y;
      }))]).range([h - 2, 2]),
      graph = d3.select('#' + settings.id)
        .append('svg').attr('class', 'sparkline').attr('width', w).attr('height', h)
        .append('g').attr('class', 'chart');

    // Line
    graph.append('path')
      .attr('class', 'line')
      .attr('d', (d3.svg.line()
        .x(function (d) { return x(d.x); })
        .y(function (d) { return y(d.y); })
      )(data));

    // Dot
    graph.append('circle')
      .attr('class', 'end')
      .attr('cx', w - 2)
      .attr('cy', y(data[data.length - 1].y))
      .attr('r', 2);

    // End value
    $('#' + settings.id).append('<span class="end-value">' + data[data.length - 1].y + '</span>');
  };
})(jQuery);
