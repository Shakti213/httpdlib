TEMPLATE = subdirs

CONFIG += build_examples

# Use experimental filesystem support when applicable
DEFINES += USE_EXPERIMENTAL_FS

build_tests {
    SUBDIRS += tests
}

build_examples {
    SUBDIRS += examples
}

listing_only {
    SUBDIRS += src
}
