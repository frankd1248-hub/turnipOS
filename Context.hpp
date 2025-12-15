#pragma once

class Kernel;
class Services;
class IO;
class PackageRegistry;

struct Context {
    Kernel& kernel;
    Services& services;
    IO& io;
    PackageRegistry& apps;
};