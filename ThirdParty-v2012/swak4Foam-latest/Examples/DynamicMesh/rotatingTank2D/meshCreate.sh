#!/bin/sh

m4 system/blockMeshDict.m4 > system/blockMeshDict
blockMesh
