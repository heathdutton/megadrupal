<div id="piechart"></div>

<script type="text/jsx">
  var PieGraph1 = React.createClass({

    loadDataFromServer: function() {
      jQuery.ajax({
        headers: {
          'Accept': 'application/json',
          'Content-Type': 'application/json'
        },
        url: this.props.url,
        dataType: 'json',
        success: function(data) {
        this.setState({ data: data });
        }.bind(this),
        error: function(xhr, status, error) {
          console.error(this.props.url, status, error.toString());
        }.bind(this)
      });
    },

    getInitialState: function() {
      return {
        data: []
      };
    },

    componentDidMount: function() {
      this.loadDataFromServer();
      setInterval(this.loadDataFromServer, this.props.pollInterval);
    },

    render: function() {
      return (
        <div className="pie-graph1">
          <h1>Data Visualization</h1>
          <Hello data={this.state.data}/>
        </div>
      );
    }
  });

  var Hello = React.createClass({
    render: function() {
      var dataNodes = this.props.data.map(function(nodes) {
        return (nodes);
      });


      var PieChart = ReactD3.PieChart;

      var tooltip = function(x, y0, y, total) {
        return y.toString();
      };

      var tooltipScatter = function(x, y) {
        return "x: " + x + " y: " + y;
      };

      var tooltipPie = function(x, y) {
        return y.toString();
      };

      var tooltipArea = function(value, cumulative) {
        return "Total: " + cumulative + " Selected: " + value;
      }


      data = {
        label: '',
        values: dataNodes
      };

      return (<PieChart
          data={data}
          width={350}
          height={120}
          margin={{top: 10, bottom: 10, left: 100, right: 100}}
          tooltipHtml={tooltipPie}
          sort={null}
          />);
    }
  });

  React.render(<PieGraph1 url="<?php print $json_url; ?>" pollInterval={5000} />,
  document.getElementById('piechart'));

</script>
<script src="http://cdnjs.cloudflare.com/ajax/libs/d3/3.5.3/d3.js"></script>
