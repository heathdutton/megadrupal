<?php
/**
 * @file
 * Theme file to handle statistic output.
 *
 * $pe is the file object.
 */
?>
<fieldset class="project-estimator-fieldset">
  <legend>The available statistics are:-</legend>
  <p><b>Total number of files:</b> <?php print $pe->totalFiles;?></p>
  <p><b>Total number of lines:</b> <?php print $pe->totalLines;?></p>
  <p><b>Total number of characters:</b> <?php print $pe->totalSize;?></p>
  <p><b>Total number of alphabetic characters:</b> <?php print $pe->totalChars;?></p>
  <p><b>Total  size in bytes:</b> <?php print $pe->totalSize;?></p>
</fieldset>
<br /><br />
<table border="1" class="project-estimator">
  <tr class="project-estimator-table-top">
    <td width="350"><b>File name</b></td>
    <td><b>Num. lines</b></td>
    <td><b>Num. chars</b></td>
    <td><b>File size</b></td>
  </tr>
  <?php $loopCount = 0;?>
  <?php foreach ($pe->files as $fname):?>
    <tr class="<?php echo ($loopCount&1) ? 'project-estimator-table-row' : 'project-estimator-table-row alt'; ?>">
      <td><?php print $fname['file_name'];?></td>
      <td><?php print $fname['num_lines'];?></td>
      <td><?php print $fname['num_chars'];?></td>
      <td><?php print $fname['file_size'];?></td>
    </tr>
    <?php $loopCount++;?>
  <?php endforeach;?>
</table>
