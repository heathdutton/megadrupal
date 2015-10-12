<div class="abs-biblesearch-versionsinatestament">
  <p><strong>Now pick a specific book:</strong></p>
  <?php
  foreach ($books as $book) {
    $book_parts = explode(':', $book->id);
    echo '<a class="book" href="#" bookid="' . $book_parts[1] . '">'. $book->name . '</a>';
  }
  ?>
</div>