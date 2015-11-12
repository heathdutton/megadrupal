/**
 * @file
 * D3 Sparkline library.
 */

(function ($) {
  Drupal.d3.sparkline = function (select, settings) {
    function maxValue(rows) {
      return d3.max(d3.merge($.extend(true, [], rows)).map(function (d) {
        return +d;
      }));
    }

    var rows = settings.rows,
      w = 100, h = 20,
      // Inset by 2 to prevent clipping.
      x = d3.scale.linear().domain([0, rows.length - 1]).range([2, w - 2]),
      y = d3.scale.linear().domain([0, maxValue(rows)]).range([h - 2, 2]),
      svg = d3.select('#' + settings.id).append('svg').attr('class', 'sparkline')
        .attr('width', w)
        .attr('height', h),
      graph = svg.append('g').attr('class', 'chart'),
      data = (settings.legend.map(function (value, index) {
        return rows.map(function (d) {
          return {x: d[0], y: +d[index + 1]};
        });
      }));

    // Line
    graph.selectAll('path.line')
      .data(data)
      .enter().append('path')
      .attr('class', 'line')
      .attr('d', d3.svg.line()
        .x(function (d, i) { return x(i); })
        .y(function (d) { return y(d.y); })
      );

    // Dot
    graph.selectAll('g.circles')
      .data(data.map(function (value) {
        return value[value.length - 1];
      }))
      .enter().append('circle')
      .attr('class', 'end')
      .attr('cx', w - 2)
      .attr('cy', function(d,i) { return y(d.y); })
      .attr('r', 2);

    // End value
    $('#' + settings.id).append('<span class="end-value">' + data[0][data[0].length - 1].y + '</span>');
  };
})(jQuery);
