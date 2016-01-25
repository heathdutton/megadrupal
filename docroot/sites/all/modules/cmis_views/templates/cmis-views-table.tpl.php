<?php
/**
 * @file
 * Theme implementation to display CMIS views in a table layout.
 */
?>
<table>
  <tr>
    <th scope="col"><?php echo $variables['tableheadings']['title']; ?></th>
    <th scope="col"><?php echo $variables['tableheadings']['description']; ?></th>
    <th scope="col"><?php echo $variables['tableheadings']['filetype']; ?></th>
    <th scope="col"><?php echo $variables['tableheadings']['lastmodified']; ?></th>
    <th scope="col"><?php echo $variables['tableheadings']['size']; ?></th>
  </tr>
  <?php foreach ($variables['rows'] as $child) : ?>
  <tr>
    <td><?php echo $child->properties['cmis:link-title']; ?></td>
    <td>
    <?php if ($child->properties['cm:description']) :
      echo $child->properties['cm:description'];
    endif; ?>
    </td>
    <td><?php echo $child->properties["cmis:contentStreamMimeType"]; ?></td>
    <td><?php echo $child->properties["cmis:lastModificationDate"]; ?></td>
    <td><?php echo $child->properties["cmis:contentStreamLength"]; ?></td>
  </tr>
  <?php endforeach; ?>
</table>
