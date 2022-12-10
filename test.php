<?php

include 'php/Binary.php';
include 'php/BinaryStream.php';

$boolValue = true;
$ubyteValue = 123;
$byteValue = -123;
$ushortValue = 12345;
$shortValue = -12345;
$utriadValue = 12345678;
$triadValue = -12345678;
$uintValue = 123456789;
$intValue = -123456789;
$ulongValue = 123456789123456789;
$longValue = -123456789123456789;
$floatValue = 0.123;
$doubleValue = 0.123456789;
$str = "Hello world";

$strBoolValue = Binary::writeByte($boolValue);
$strUByteValue = Binary::writeByte($ubyteValue);
$strByteValue = Binary::writeByte($byteValue);
$strUShortValue = Binary::writeShort($ushortValue);
$strShortValue = Binary::writeShort($shortValue);
$strLShortValue = Binary::writeLShort($shortValue);
$strUTriadValue = Binary::writeTriad($utriadValue);
$strTriadValue = Binary::writeTriad($triadValue);
$strLTriadValue = Binary::writeLTriad($triadValue);
$strUIntValue = Binary::writeInt($uintValue);
$strIntValue = Binary::writeInt($intValue);
$strLIntValue = Binary::writeLInt($intValue);
$strULongValue = Binary::writeLong($ulongValue);
$strLongValue = Binary::writeLong($longValue);
$strLLongValue = Binary::writeLLong($longValue);
$strUVarIntValue = Binary::writeUnsignedVarInt($uintValue);
$strVarIntValue = Binary::writeVarInt($intValue);
$strUVarLongValue = Binary::writeUnsignedVarLong($ulongValue);
$strVarLongValue = Binary::writeVarLong($longValue);
$strFloatValue = Binary::writeFloat($floatValue);
$strLFloatValue = Binary::writeLFloat($floatValue);
$strDoubleValue = Binary::writeDouble($doubleValue);
$strLDoubleValue = Binary::writeLDouble($doubleValue);

var_dump(Binary::writeBool($boolValue) ===  \pocketmine\utils\Binary::writeBool($boolValue));
var_dump(Binary::writeByte($byteValue) ===  \pocketmine\utils\Binary::writeByte($byteValue));
var_dump(Binary::writeShort($shortValue) ===  \pocketmine\utils\Binary::writeShort($shortValue));
var_dump(Binary::writeLShort($shortValue) ===  \pocketmine\utils\Binary::writeLShort($shortValue));
var_dump(Binary::writeTriad($triadValue) ===  \pocketmine\utils\Binary::writeTriad($triadValue));
var_dump(Binary::writeLTriad($triadValue) ===  \pocketmine\utils\Binary::writeLTriad($triadValue));
var_dump(Binary::writeInt($intValue) ===  \pocketmine\utils\Binary::writeInt($intValue));
var_dump(Binary::writeLInt($intValue) ===  \pocketmine\utils\Binary::writeLInt($intValue));
var_dump(Binary::writeLong($longValue) ===  \pocketmine\utils\Binary::writeLong($longValue));
var_dump(Binary::writeLLong($longValue) ===  \pocketmine\utils\Binary::writeLLong($longValue));
var_dump(Binary::writeVarInt($intValue) ===  \pocketmine\utils\Binary::writeVarInt($intValue));
var_dump(Binary::writeUnsignedVarInt($intValue) ===  \pocketmine\utils\Binary::writeUnsignedVarInt($intValue));
var_dump(Binary::writeVarLong($longValue) ===  \pocketmine\utils\Binary::writeVarLong($longValue));
var_dump(Binary::writeUnsignedVarLong($longValue) ===  \pocketmine\utils\Binary::writeUnsignedVarLong($longValue));
var_dump(Binary::writeFloat($floatValue) ===  \pocketmine\utils\Binary::writeFloat($floatValue));
var_dump(Binary::writeLFloat($floatValue) ===  \pocketmine\utils\Binary::writeLFloat($floatValue));
var_dump(Binary::writeDouble($doubleValue) ===  \pocketmine\utils\Binary::writeDouble($doubleValue));
var_dump(Binary::writeLDouble($doubleValue) ===  \pocketmine\utils\Binary::writeLDouble($doubleValue));

var_dump(Binary::readBool($strBoolValue) ===  \pocketmine\utils\Binary::readBool($strBoolValue));
var_dump(Binary::readByte($strUByteValue) ===  \pocketmine\utils\Binary::readByte($strUByteValue));
var_dump(Binary::readSignedByte($strByteValue) ===  \pocketmine\utils\Binary::readSignedByte($strByteValue));
var_dump(Binary::readShort($strUShortValue) ===  \pocketmine\utils\Binary::readShort($strUShortValue));
var_dump(Binary::readSignedShort($strShortValue) ===  \pocketmine\utils\Binary::readSignedShort($strShortValue));
var_dump(Binary::readLShort($strLShortValue) ===  \pocketmine\utils\Binary::readLShort($strLShortValue));
var_dump(Binary::readTriad($strTriadValue) ===  \pocketmine\utils\Binary::readTriad($strTriadValue));
var_dump(Binary::readLTriad($strLTriadValue) ===  \pocketmine\utils\Binary::readLTriad($strLTriadValue));
var_dump(Binary::readInt($strUIntValue) ===  \pocketmine\utils\Binary::readInt($strUIntValue));
var_dump(Binary::readLInt($strLIntValue) ===  \pocketmine\utils\Binary::readLInt($strLIntValue));
var_dump(Binary::readLong($strULongValue) ===  \pocketmine\utils\Binary::readLong($strULongValue));
var_dump(Binary::readLLong($strLLongValue) ===  \pocketmine\utils\Binary::readLLong($strLLongValue));
$offset1 = 0;
$offset2 = 0;
var_dump(Binary::readVarInt($strVarIntValue, $offset1) ===  \pocketmine\utils\Binary::readVarInt($strVarIntValue, $offset2));
var_dump($offset1 === $offset2);
$offset1 = 0;
$offset2 = 0;
var_dump(Binary::readUnsignedVarInt($strUVarIntValue, $offset1) ===  \pocketmine\utils\Binary::readUnsignedVarInt($strUVarIntValue, $offset2));
var_dump($offset1 === $offset2);
$offset1 = 0;
$offset2 = 0;
var_dump(Binary::readVarLong($strVarLongValue, $offset1) ===  \pocketmine\utils\Binary::readVarLong($strVarLongValue, $offset2));
var_dump($offset1 === $offset2);
$offset1 = 0;
$offset2 = 0;
var_dump(Binary::readUnsignedVarLong($strUVarLongValue, $offset1) ===  \pocketmine\utils\Binary::readUnsignedVarLong($strUVarLongValue, $offset2));
var_dump($offset1 === $offset2);
var_dump(Binary::readFloat($strFloatValue) ===  \pocketmine\utils\Binary::readFloat($strFloatValue));
var_dump(Binary::readLFloat($strLFloatValue) ===  \pocketmine\utils\Binary::readLFloat($strLFloatValue));
var_dump(Binary::readDouble($strDoubleValue) ===  \pocketmine\utils\Binary::readDouble($strDoubleValue));
var_dump(Binary::readLDouble($strLDoubleValue) ===  \pocketmine\utils\Binary::readLDouble($strLDoubleValue));

$phpStream = new \pocketmine\utils\BinaryStream();
$cppStream = new BinaryStream();


for ($i = 0; $i < 10; $i++) {
    $phpStream->putBool($boolValue);
    $cppStream->putBool($boolValue);
    var_dump($cppStream->getBuffer() === $phpStream->getBuffer());
    var_dump($cppStream->getOffset() === $phpStream->getOffset());

    $phpStream->putByte($byteValue);
    $cppStream->putByte($byteValue);
    var_dump($cppStream->getBuffer() === $phpStream->getBuffer());
    var_dump($cppStream->getOffset() === $phpStream->getOffset());

    $phpStream->putShort($shortValue);
    $cppStream->putShort($shortValue);
    var_dump($cppStream->getBuffer() === $phpStream->getBuffer());
    var_dump($cppStream->getOffset() === $phpStream->getOffset());

    $phpStream->putLShort($shortValue);
    $cppStream->putLShort($shortValue);
    var_dump($cppStream->getBuffer() === $phpStream->getBuffer());
    var_dump($cppStream->getOffset() === $phpStream->getOffset());

    $phpStream->putTriad($intValue);
    $cppStream->putTriad($intValue);
    var_dump($cppStream->getBuffer() === $phpStream->getBuffer());
    var_dump($cppStream->getOffset() === $phpStream->getOffset());

    $phpStream->putLTriad($intValue);
    $cppStream->putLTriad($intValue);
    var_dump($cppStream->getBuffer() === $phpStream->getBuffer());
    var_dump($cppStream->getOffset() === $phpStream->getOffset());

    $phpStream->putInt($triadValue);
    $cppStream->putInt($triadValue);
    var_dump($cppStream->getBuffer() === $phpStream->getBuffer());
    var_dump($cppStream->getOffset() === $phpStream->getOffset());

    $phpStream->putLInt($triadValue);
    $cppStream->putLInt($triadValue);
    var_dump($cppStream->getBuffer() === $phpStream->getBuffer());
    var_dump($cppStream->getOffset() === $phpStream->getOffset());

    $phpStream->putLong($longValue);
    $cppStream->putLong($longValue);
    var_dump($cppStream->getBuffer() === $phpStream->getBuffer());
    var_dump($cppStream->getOffset() === $phpStream->getOffset());

    $phpStream->putLLong($longValue);
    $cppStream->putLLong($longValue);
    var_dump($cppStream->getBuffer() === $phpStream->getBuffer());
    var_dump($cppStream->getOffset() === $phpStream->getOffset());

    $phpStream->putVarInt($intValue);
    $cppStream->putVarInt($intValue);
    var_dump($cppStream->getBuffer() === $phpStream->getBuffer());
    var_dump($cppStream->getOffset() === $phpStream->getOffset());

    $phpStream->putUnsignedVarInt($uintValue);
    $cppStream->putUnsignedVarInt($uintValue);
    var_dump($cppStream->getBuffer() === $phpStream->getBuffer());
    var_dump($cppStream->getOffset() === $phpStream->getOffset());

    $phpStream->putVarLong($longValue);
    $cppStream->putVarLong($longValue);
    var_dump($cppStream->getBuffer() === $phpStream->getBuffer());
    var_dump($cppStream->getOffset() === $phpStream->getOffset());

    $phpStream->putUnsignedVarLong($ulongValue);
    $cppStream->putUnsignedVarLong($ulongValue);
    var_dump($cppStream->getBuffer() === $phpStream->getBuffer());
    var_dump($cppStream->getOffset() === $phpStream->getOffset());

    $phpStream->putFloat($floatValue);
    $cppStream->putFloat($floatValue);
    var_dump($cppStream->getBuffer() === $phpStream->getBuffer());
    var_dump($cppStream->getOffset() === $phpStream->getOffset());

    $phpStream->putLFloat($floatValue);
    $cppStream->putLFloat($floatValue);
    var_dump($cppStream->getBuffer() === $phpStream->getBuffer());
    var_dump($cppStream->getOffset() === $phpStream->getOffset());

    $phpStream->putDouble($floatValue);
    $cppStream->putDouble($floatValue);
    var_dump($cppStream->getBuffer() === $phpStream->getBuffer());
    var_dump($cppStream->getOffset() === $phpStream->getOffset());

    $phpStream->putLDouble($floatValue);
    $cppStream->putLDouble($floatValue);
    var_dump($cppStream->getBuffer() === $phpStream->getBuffer());
    var_dump($cppStream->getOffset() === $phpStream->getOffset());

    $phpStream->put($str);
    $cppStream->put($str);
    var_dump($cppStream->getBuffer() === $phpStream->getBuffer());
    var_dump($cppStream->getOffset() === $phpStream->getOffset());

    var_dump($cppStream->get(5) === $phpStream->get(5));
    var_dump($cppStream->getOffset() === $phpStream->getOffset());
    var_dump($cppStream->feof() === $phpStream->feof());
    var_dump($cppStream->getRemaining() === $phpStream->getRemaining());
    var_dump($cppStream->getOffset() === $phpStream->getOffset());
    var_dump($cppStream->feof() === $phpStream->feof());

    
    $phpStream->putUnsignedVarInt(123456);
    $cppStream->putUnsignedVarInt(123456);
    echo $phpStream->getUnsignedVarInt() . "\n";
    echo $cppStream->getUnsignedVarInt();
}
?>