var gulp = require('gulp');
var gutil = require('gulp-util');
var sass = require('gulp-sass');
var watch = require('gulp-watch');
var shell = require('gulp-shell');
var notify = require('gulp-notify');
var browserSync = require('browser-sync');
var sourcemaps = require('gulp-sourcemaps');
var uglify = require('gulp-uglify');

gulp.task('sass', function () {
  return gulp.src('./scss/**/*.scss')
    .pipe(sourcemaps.init())
    .pipe(sass({
      noCache: true,
      outputStyle: "compressed",
      lineNumbers: false,
      loadPath: './css/*',
      sourceMap: true
    }))
    .pipe(sourcemaps.write('./maps'))
    .pipe(gulp.dest('./css'))
    .pipe(notify({
      title: "SASS Compiled",
      message: "All SASS files have been recompiled to CSS.",
      onLast: true
    }));
});

gulp.task('compress', function() {
  return gulp.src('./js-src/*.js')
    .pipe(sourcemaps.init())
    .pipe(uglify())
    .pipe(sourcemaps.write('./maps'))
    .pipe(gulp.dest('./js'))
    .pipe(notify({
      title: "JS Minified",
      message: "All JS files in the theme have been minified.",
      onLast: true
    }));
});

gulp.task('drush:cc', function () {
  return gulp.src('', {read: false})
    .pipe(shell([
      'drush cc css-js',
    ]))
    .pipe(notify({
      title: "Caches cleared",
      message: "Drupal CSS/JS caches cleared.",
      onLast: true
    }));
});

gulp.task('watch', function() {
  browserSync({
    proxy: "ENTER YOUR SITE HOSTNAME"
  });

  // watch scss, js, and tpl files and clear drupal theme cache on change, reload browsers
  gulp.watch(['scss/**/*.scss', 'js-src/**/*.js', 'templates/**/*.tpl.php'], function() {
    gulp.run('sass');
    gulp.run('compress');
    gulp.run('drush:cc');
  }).on("change", browserSync.reload);
});

gulp.task('default', ['watch']);