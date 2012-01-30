<?php
/**
 * @file
 * Signaturefield element template file
 */
?>
<label for="<?php echo $element['#id'] ?>"><?php echo $element['#title'] ?></label>
<p class='drawItDesc'></p>
<div class='sig sigWrapper'>
  <div class='typed'></div>
  <canvas class='pad' width='<?php echo $element['#width'] ?>' height='<?php echo $element['#height'] ?>'></canvas>
  <input type='hidden' name='<?php echo $element['#name'] ?>' id='<?php echo $element['#id'] ?>' class='output' value='<?php if(isset($element['#default_value'])) { echo $element['#default_value']; } ?>'>
  <br /><a href='#clear' class='clearButton'>Clear signature</a>
</div>
<?php if (!empty($element['#description']))  echo "<div class='description'>" . $element['#description'] . "</div>"; ?>
