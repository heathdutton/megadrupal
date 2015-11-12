  <?php print render($page['header']); ?>

  <div id="wrapper">
    <div id="container" class="clearfix">

      <div id="center"><div id="squeeze"><div class="right-corner"><div class="left-corner">
          <?php print $messages; ?>
          <div class="clearfix">
            <?php print render($page['content']); ?>
          </div>
      </div></div></div></div> <!-- /.left-corner, /.right-corner, /#squeeze, /#center -->

    </div> <!-- /#container -->
  </div> <!-- /#wrapper -->
