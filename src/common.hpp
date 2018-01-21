#pragma once

enum ObjectType {
  ObjectType_None = 0x00,
  ObjectType_Model = 0x01,
  ObjectType_Grid = 0x01 << 1,
  ObjectType_Camera = 0x01 << 2,
  ObjectType_Gizmo = 0x01 << 3,
};
