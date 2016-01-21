<div id="relevant-content-admin-overview">
<?php foreach ($rows as $row) : ?>
  <table class="<?php print $row['row_classes_rendered'] ?>">
    <tbody>
      <tr>
        <td class="relevant-content-block-name">
          <?php print t('<em>@status</em> <strong>@id</strong>', array('@status' => $row['status'], '@id' => $row['block']['id'])); ?>
        </td>
        <td class="relevant-content-ops">
          <?php print $row['ops_rendered'] ?>
        </td>
      </tr>

      <tr>
        <td>
          <div class="relevant-content-types"><?php print t('Content Types: !types', array('!types' => $row['types_rendered'])); ?></div>
          <div class="relevant-content-vocabs"><?php print t('Vocabularies: !vocabs', array('!vocabs' => $row['vocabs_rendered'])); ?></div>
          <div class="relevant-content-max-items"><?php print t('Max Items: !max_items', array('!max_items' => $row['block']['max_items'])); ?></div>
          <div class="relevant-content-links"><?php print t('Link Type: !absolute_links', array('!absolute_links' => $row['absolute_links_rendered'])); ?></div>
        </td>
        <td class="relevant-content-header">
          <div class="relevant-content-header"><?php print $row['header_rendered']; ?></div>
        </td>
      </tr>
    </tbody>
  </table>
<?php endforeach; ?>
</div>
