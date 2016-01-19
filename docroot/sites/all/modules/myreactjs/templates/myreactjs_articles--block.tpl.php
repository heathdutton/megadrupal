<div id="myreactjs-content"></div>

<script type="text/jsx">
  var CommentBox = React.createClass({

    loadCommentsFromServer: function() {
      jQuery.ajax({
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
      this.loadCommentsFromServer();
      setInterval(this.loadCommentsFromServer, this.props.pollInterval);
    },

    render: function() {
      return (
        <div className="commentBox">
          <h1>Contents</h1>
          <CommentList data={this.state.data} />
        </div>
      );
    }

  });

  var CommentList = React.createClass({
    render: function() {
      var commentNodes = this.props.data.map(function(comment) {
        return (
          <Comment title={comment.title} body={comment.Body} image={comment.Image.src}>
          </Comment>
        );
      });

      return (
        <div className="commentList">
          {commentNodes}
        </div>
      );
    }
  });


  var Comment = React.createClass({
    render: function() {
      return (
        <div className="comment">
          <h2 className="commentAuthor">
            {this.props.title}
          </h2>
          <img src={this.props.image}/>
          <span dangerouslySetInnerHTML={{__html: this.props.body}} />
        </div>
      );
    }
  });

  React.render(<CommentBox url="<?php print $json_url ?>" pollInterval={5000} />,
  document.getElementById('myreactjs-content'));
</script>
