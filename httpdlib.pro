TEMPLATE = subdirs

CONFIG += build_examples

build_tests {
    SUBDIRS += tests
}

build_examples {
    SUBDIRS += examples
}

listing_only {
    SUBDIRS += src
}
