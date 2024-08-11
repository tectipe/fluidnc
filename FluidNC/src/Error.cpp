// Copyright (c) 2014-2016 Sungeun K. Jeon for Gnea Research LLC
// Copyright (c) 2018 -	Bart Dring
// Copyright (c) 2020 - Mitch Bradley
// Use of this source code is governed by a GPLv3 license that can be found in the LICENSE file.

#include "Error.h"

const std::map<Error, const char*> ErrorNames = {
    { Error::Ok, "No error" },
    { Error::ExpectedCommandLetter, "Expected GCodecommand letter" },
    { Error::BadNumberFormat, "Bad GCode number format" },
    { Error::InvalidStatement, "Invalid $ statement" },
    { Error::NegativeValue, "Negative value" },
    { Error::SettingDisabled, "Setting disabled" },
    { Error::SettingStepPulseMin, "Step pulse too short" },
    { Error::SettingReadFail, "Failed to read settings" },
    { Error::IdleError, "Command requires idle state" },
    { Error::SystemGcLock, "GCode cannot be executed in lock or alarm state" },
    { Error::SoftLimitError, "Soft limit error" },
    { Error::Overflow, "Line too long" },
    { Error::MaxStepRateExceeded, "Max step rate exceeded" },
    { Error::CheckDoor, "Check door" },
    { Error::LineLengthExceeded, "Startup line too long" },
    { Error::TravelExceeded, "Max travel exceeded during jog" },
    { Error::InvalidJogCommand, "Invalid jog command" },
    { Error::SettingDisabledLaser, "Laser mode requires PWM output" },
    { Error::HomingNoCycles, "No Homing/Cycle defined in settings" },
    { Error::SingleAxisHoming, "Single axis homing not allowed" },
    { Error::GcodeUnsupportedCommand, "Unsupported GCode command" },
    { Error::GcodeModalGroupViolation, "Gcode modal group violation" },
    { Error::GcodeUndefinedFeedRate, "Gcode undefined feed rate" },
    { Error::GcodeCommandValueNotInteger, "Gcode command value not integer" },
    { Error::GcodeAxisCommandConflict, "Gcode axis command conflict" },
    { Error::GcodeWordRepeated, "Gcode word repeated" },
    { Error::GcodeNoAxisWords, "Gcode no axis words" },
    { Error::GcodeInvalidLineNumber, "Gcode invalid line number" },
    { Error::GcodeValueWordMissing, "Gcode value word missing" },
    { Error::GcodeUnsupportedCoordSys, "Gcode unsupported coordinate system" },
    { Error::GcodeG53InvalidMotionMode, "Gcode G53 invalid motion mode" },
    { Error::GcodeAxisWordsExist, "Gcode extra axis words" },
    { Error::GcodeNoAxisWordsInPlane, "Gcode no axis words in plane" },
    { Error::GcodeInvalidTarget, "Gcode invalid target" },
    { Error::GcodeArcRadiusError, "Gcode arc radius error" },
    { Error::GcodeNoOffsetsInPlane, "Gcode no offsets in plane" },
    { Error::GcodeUnusedWords, "Gcode unused words" },
    { Error::GcodeG43DynamicAxisError, "Gcode G43 dynamic axis error" },
    { Error::GcodeMaxValueExceeded, "Gcode max value exceeded" },
    { Error::PParamMaxExceeded, "P param max exceeded" },
    { Error::CheckControlPins, "Check control pins" },
    { Error::FsFailedMount, "Failed to mount device" },
    { Error::FsFailedRead, "Read failed" },
    { Error::FsFailedOpenDir, "Failed to open directory" },
    { Error::FsDirNotFound, "Directory not found" },
    { Error::FsFileEmpty, "File empty" },
    { Error::FsFileNotFound, "File not found" },
    { Error::FsFailedOpenFile, "Failed to open file" },
    { Error::FsFailedBusy, "Device is busy" },
    { Error::FsFailedDelDir, "Failed to delete directory" },
    { Error::FsFailedDelFile, "Failed to delete file" },
    { Error::FsFailedRenameFile, "Failed to rename file" },
    { Error::FsFailedCreateFile, "Failed to create file" },
    { Error::FsFailedFormat, "Failed to format filesystem" },
    { Error::NumberRange, "Number out of range for setting" },
    { Error::InvalidValue, "Invalid value for setting" },
    { Error::MessageFailed, "Failed to send message" },
    { Error::NvsSetFailed, "Failed to store setting" },
    { Error::NvsGetStatsFailed, "Failed to get setting status" },
    { Error::AuthenticationFailed, "Authentication failed!" },
    { Error::Eol, "End of line" },
    { Error::Eof, "End of file" },
    { Error::Reset, "System Reset" },
    { Error::NoData, "No Data" },
    { Error::AnotherInterfaceBusy, "Another interface is busy" },
    { Error::BadPinSpecification, "Bad Pin Specification" },
    { Error::BadRuntimeConfigSetting, "Bad Runtime Config Setting" },
    { Error::JogCancelled, "Jog Cancelled" },
    { Error::ConfigurationInvalid, "Configuration is invalid. Check boot messages for ERR's." },
    { Error::UploadFailed, "File Upload Failed" },
    { Error::DownloadFailed, "File Download Failed" },
    { Error::ReadOnlySetting, "Read-only setting" },
    { Error::ExpressionDivideByZero, "Expression Divide By Zero" },
    { Error::ExpressionInvalidArgument, "Expression Invalid Argument" },
    { Error::ExpressionUnknownOp, "Expression Unknown Operator" },
    { Error::ExpressionArgumentOutOfRange, "Expression Argument Out of Range" },
    { Error::ExpressionSyntaxError, "Expression Syntax Error" },
    { Error::FlowControlSyntaxError, "Flow Control Syntax Error" },
    { Error::FlowControlNotExecutingMacro, "Flow Control Not Executing Macro" },
    { Error::FlowControlOutOfMemory, "Flow Control Out of Memory" },
    { Error::FlowControlStackOverflow, "Flow Control Stack Overflow" },
};
