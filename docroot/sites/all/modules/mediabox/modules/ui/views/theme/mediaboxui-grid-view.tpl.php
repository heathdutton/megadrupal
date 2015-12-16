<?php
/**
 * Browse view template.
 */
?>
<div id="mediabox-ui-library">
  <div id="mediabox-ui-library-wrapper">
    <div id="mediabox-ui-library-content">
      <div id="mediabox-ui-library-content-wrapper">
        <div id="mediabox-ui-library-grid-wrapper">
          <table>
            <tbody>
              <?php foreach ($rows as $delta => $row): ?>
                <tr>
                  <?php foreach ($row as $record): ?>
                    <td>
                      <div id="<?php print $record['id']; ?>" class="mediabox-selectable">
                        <?php print $record['record']; ?>
                      </div>
                    </td>
                  <?php endforeach; ?>
                </tr>
              <?php endforeach; ?>
            </tbody>
          </table>
        </div>
      </div>
    </div>
  </div>
</div>
