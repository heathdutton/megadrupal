(function($) {

Drupal.behaviors.tribune_geoflags = {
  attach: function(context) {
    $('#tribune-stats-countries', context).each(function() {
      var width = 600,
          height = 400;

      // I like hammer because it show Antarctica
      var proj = d3.geo.hammer()
        .translate([width/2, height/2])
        .scale(120);

      var path = d3.geo.path().projection(proj);

      var t = proj.translate();
      var s = proj.scale();

      var map = d3.select($(this).get(0)).append("svg")
          .attr("width", width)
          .attr("height", height);

      var counts = d3.map(Drupal.settings.tribune_stats.countries.counts);

      var color_scale = d3.scale.log()
          .domain([1, d3.max(counts.values())])
          .range(['white', 'green']);

      var color = function(code) {
        var count = counts.get(code);
        if (count) {
          return color_scale(count);
        } else {
          return '#fff';
        }
      };

      var countries = map.append("svg:g").attr("id", "countries");

      d3.json(Drupal.settings.tribune_stats.countries.data, function (json) {
        countries.selectAll("path")
            .data(topojson.object(json, json.objects.countries).geometries)
          .enter().append("svg:path")
            .style('fill', function(d) {return color(d.id);})
            .style('stroke', '#000')
            .attr("d", path)
          .append('title')
            .text(function(d) {
              var posts = counts.get(d.id);
              return Drupal.t('!country: !posts posts.', {
                '!country': d.properties.name,
                '!posts': posts ? posts : 0
              });
            });
      });
    });
  }
};

})(jQuery);
