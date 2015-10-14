(function($) {

Drupal.tribune_local = {};

Drupal.behaviors.tribune_local_stats = {
  attach: function(context) {
    $('#tribune-stats-activity', context).each(function() {
      var width = 570,
          height = 80,
          cellSize = 10; // cell size

      var day = function(a) { return ((parseInt(d3.time.format("%w")(a)) + 6 ) % 7); },
                              // we need monday as 0, no sunday
          week = d3.time.format("%W"),
          format = d3.time.format("%Y%m%d");

      var log_scale = d3.scale.log()
          .domain([Drupal.settings.tribune_stats.activity.min, Drupal.settings.tribune_stats.activity.max])
          .range([0, 1]);

      var color_scale = d3.scale.linear()
          .domain([0, 0.5, 1])
          .range(['green', 'yellow', 'red']);

      var color = function(a) {return color_scale(log_scale(a));};

      var svg = d3.select($(this).get(0)).selectAll("svg")
          .data(d3.range(Drupal.settings.tribune_stats.activity.start.year, Drupal.settings.tribune_stats.activity.stop.year + 1))
        .enter().append("svg")
          .attr("width", width)
          .attr("height", height)
        .append("g")
          .attr("transform", "translate(" + ((width - cellSize * 53) / 2) + "," + (height - cellSize * 7 - 1) + ")");

      svg.append("text")
          .attr("transform", "translate(-6," + cellSize * 3.5 + ")rotate(-90)")
          .style("text-anchor", "middle")
          .text(function(d) { return d; });

      var rect = svg.selectAll(".day")
          .data(function(d) { return d3.time.days(new Date(d, 0, 1), new Date(d + 1, 0, 1)); })
        .enter().append("rect")
          .attr("class", "day")
          .attr("width", cellSize)
          .attr("height", cellSize)
          .attr("x", function(d) { return week(d) * cellSize; })
          .attr("y", function(d) { return day(d) * cellSize; })
          .datum(format);

      rect.append("title")
          .text(function(d) { return d; });

      svg.selectAll(".month")
          .data(function(d) { return d3.time.months(new Date(d, 0, 1), new Date(d + 1, 0, 1)); })
        .enter().append("path")
          .attr("class", "month")
          .attr("d", monthPath);

      var data = d3.nest()
        .key(function(d) { return d.date; })
        .rollup(function(d) { return d[0].count; })
        .map(Drupal.settings.tribune_stats.activity.calendar);

      rect.filter(function(d) { return d in data; })
          .attr("class", "day")
          .attr("style", function(d) { return "fill: " + color(data[d]); })
        .select("title")
          .text(function(d) { return d + ": " + data[d]; });

      function monthPath(t0) {
        var t1 = new Date(t0.getFullYear(), t0.getMonth() + 1, 0),
            d0 = +day(t0), w0 = +week(t0),
            d1 = +day(t1), w1 = +week(t1);
        return "M" + (w0 + 1) * cellSize + "," + d0 * cellSize
            + "H" + w0 * cellSize + "V" + 7 * cellSize
            + "H" + w1 * cellSize + "V" + (d1 + 1) * cellSize
            + "H" + (w1 + 1) * cellSize + "V" + 0
            + "H" + (w0 + 1) * cellSize + "Z";
      }
    });

    $('#tribune-stats-daily-activity', context).each(function() {
      var cellSize = 30;
      var offset = 70;
      var width = cellSize * 25 + offset,
          height = cellSize * 8;

      var day = function(a) { return ((parseInt(d3.time.format("%w")(a)) + 6 ) % 7); },
                              // we need monday as 0, no sunday
          week = d3.time.format("%W"),
          format = d3.time.format("%Y%m%d");

      var data = d3.nest()
        .key(function(d) { return d.division; })
        .rollup(function(d) { return d[0]; })
        .map(Drupal.settings.tribune_stats.activity.hourly, d3.map);

      var weekday = function(i) {
        var days = [
            Drupal.t('Monday'),
            Drupal.t('Tuesday'),
            Drupal.t('Wednesday'),
            Drupal.t('Thursday'),
            Drupal.t('Friday'),
            Drupal.t('Saturday'),
            Drupal.t('Sunday'),
        ];

        return days[i];
      };


      var min = d3.min(Drupal.settings.tribune_stats.activity.hourly, function(d) {return parseInt(d.count)});
      var max = d3.max(Drupal.settings.tribune_stats.activity.hourly, function(d) {return parseInt(d.count)});
      var log_scale = d3.scale.linear()
          .domain([min, max])
          .range([0, 1]);

      color_scale = d3.scale.linear()
          .domain([0, 0.1, 0.5, 1])
          .range(['white', 'green', 'yellow', 'red']);

      var color = function(d) {
          var weekday = d.weekday.toString();
          var hour = d.hour.toString();
          if (hour.length < 2) { hour = "0" + hour; }
          var index = weekday + ':' + hour;
          return color_scale(log_scale(data[index].count));
      };

      var label = function(d) {
          var weekday = d.weekday.toString();
          var hour = d.hour.toString();
          if (hour.length < 2) { hour = "0" + hour; }
          var index = weekday + ':' + hour;
          return data[index].count;
      };

      var divisions = function() {
        var cells = Array();
        for (var day = 0; day <= 6; day++) {
            for (var hour = 0; hour <= 23; hour++) {
                cells.push({
                    weekday: day,
                    hour: hour
                });
            }
        }
        return cells;
      };

      var svg = d3.select($(this).get(0)).selectAll("svg")
          .data([0])
        .enter().append("svg")
          .attr("width", width)
          .attr("height", height)
        .append("g");

      var rect = svg.selectAll(".day")
          .data(divisions())
        .enter().append("rect")
          .attr("class", "day")
          .attr("width", cellSize)
          .attr("height", cellSize)
          .attr("x", function(d) { return (1+parseInt(d.hour)) * cellSize + offset; })
          .attr("y", function(d) { return (1+parseInt(d.weekday)) * cellSize; });

      rect.attr("style", function(d) { return "fill: " + color(d); })
        .append("title")
          .text(function(d) { return label(d); });

      svg.selectAll(".hours-axis")
          .data(d3.range(0, 24))
        .enter().append("text")
          .attr("class", "label")
          .style("text-anchor", "middle")
          .attr("x", function(d) { return (1 + d) * cellSize + cellSize/2 + offset; })
          .attr("y", cellSize/1.5)
          .text(function(d) { return d; });

      svg.append("text")
          .attr("class", "label utc")
          .style("text-anchor", "start")
          .attr("x", 0)
          .attr("y", cellSize/1.5)
          .text(Drupal.t('UTC'));

      svg.selectAll(".days-axis")
          .data(d3.range(0, 7))
        .enter().append("text")
          .attr("class", "label")
          .style("text-anchor", "end")
          .attr("x", cellSize/1.5 + offset)
          .attr("y", function(d) { return (1 + d) * cellSize + cellSize/1.5; })
          .text(function(d) { return weekday(d); });
    });

    $('#tribune-stats-users', context).each(function() {
      var width = 300,
          height = 300;

      var color = d3.scale.ordinal()
          .range(["#98abc5", "#8a89a6", "#7b6888", "#6b486b", "#a05d56", "#d0743c", "#ff8c00"]);

      var pie = d3.layout.pie()
          .sort(null)
          .value(function(d) { return d.count; });

      var total = 0;
      for (var key in Drupal.settings.tribune_stats.users) {
          var data = d3.map(Drupal.settings.tribune_stats.users[key]);
          total += d3.sum(data.values(), function(a) {return a.count;});
      }

      radius_scale = d3.scale.linear()
          .domain([0, 10, total])
          .range([0, Math.min(width, height) / 8, Math.min(width, height) / 2]);

      for (var key in Drupal.settings.tribune_stats.users) {
          var radius = Math.min(width, height) / 2;

          var data = d3.map(Drupal.settings.tribune_stats.users[key]);
          var counts = data.values().map(function(a) {return a.count;}).sort(d3.ascending);
          var cutoff = d3.quantile(counts, 0.995);

          var sum = d3.sum(data.values(), function(a) {return a.count;});
          radius = radius_scale(sum);

          var arc = d3.svg.arc()
              .outerRadius(radius - 20)
              .innerRadius(0);


          var nest = d3.nest()
              .key(function(d) {return d.count <= cutoff ? 'others' : Math.random();})
              .rollup(function(d) {
                if (d.length > 1) {
                    var sum = d.reduce(function(prev, curr, i) {
                        return {count: prev.count + curr.count};
                    });
                    return {
                        count: sum.count,
                        name: Drupal.t('Others'),
                        grouped: d.length
                    }
                } else {
                    d[0].grouped = 0;
                    return d[0];
                }
              })
              .entries(data.values()).map(function(a) {return a.values;});

          data = nest.sort(function(a, b) {
                // sort grouped users after
                if (a.grouped && !b.grouped) {
                  return -1;
                } else if (!a.grouped && b.grouped) {
                  return 1;
                }

                return d3.ascending(a.count, b.count);
              }
          );

          var svg = d3.select($(this).get(0)).append("svg")
              .attr("width", width)
              .attr("height", height)
            .append("g")
              .attr("transform", "translate(" + width / 2 + "," + height / 2 + ")");

          var g = svg.selectAll(".arc")
              .data(pie(data))
            .enter().append("g")
              .attr("class", "arc user");

          g.append("path")
              .attr("d", arc)
              .style("fill", function(d) { return color(d.data.count); });

          g.append("text")
              .attr("transform", function(d) { return "translate(" + arc.centroid(d) + ")"; })
              .attr("dy", ".35em")
              .style("text-anchor", "middle")
              .text(function(d) { return d.data.name; });

          g.append("title")
              .text(function(d) { return Drupal.t('!count messages', {'!count': d.data.count}); });

          svg.append("text")
              .style("text-anchor", "middle")
              .attr('y', -1 * height/2 + 15)
              .text(key);
      }
    });

    $('#tribune-stats-relations', context).each(function() {
      var width = 600,
          height = 500;

      var color = d3.scale.category20();

      force = d3.layout.force()
          .charge(-120)
          .linkDistance(function(d) {return nodeRadius(d.source.count) + nodeRadius(d.target.count) + 100;})
          .size([width, height]);

      var svg = d3.select($(this).get(0)).append("svg")
          .attr("width", width)
          .attr("height", height);

      var graph = Drupal.settings.tribune_stats.relations;
      names = d3.map(graph.names).values();
      relations = graph.relations;

      map = d3.map();

      names.forEach(function(d, i) {
        d.weight = 0;
        map.set(d.uid, i);
      });

      relations.forEach(function(d, i) {
        d.source = map.get(d.source_id);
        d.target = map.get(d.target_id);
      });

      scale = d3.scale.linear()
          .domain([d3.min(relations, function(d) {return d.count}), d3.max(relations, function(d) {return d.count}), d3.max(names, function(d) {return parseInt(d.count)})])
          .range([5, 20, 40]);

      var nodeRadius = function(count) {
        return scale(parseInt(count));
      }

      var linkWidth = function(count) {
        return scale(parseInt(count));
      }


      force.nodes(names)
          .links(relations)
          .start();

      var link = svg.selectAll(".link")
          .data(relations)
        .enter().append("line")
          .attr("class", "link")
          .style("stroke", "#999")
          .style("stroke-width", function(d) { return linkWidth(d.count); });

      var node = svg.selectAll(".node")
          .data(names)
        .enter().append("circle")
          .attr("class", "node")
          .attr("r", function(d) { return nodeRadius(d.count); })
          .style("fill", '#ABCDEF')
          .call(force.drag);

      node.append("title")
          .text(function(d) { return d.name; });

      force.on("tick", function() {
        link.attr("x1", function(d) { return d.source.x; })
            .attr("y1", function(d) { return d.source.y; })
            .attr("x2", function(d) { return d.target.x; })
            .attr("y2", function(d) { return d.target.y; });

        node.attr("cx", function(d) { return d.x; })
            .attr("cy", function(d) { return d.y; });
      });
    });
  }
};

})(jQuery);
