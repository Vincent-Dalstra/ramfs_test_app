These components are intended to be automatically managed, but because we're developing them
we may need to edit them and push changes on git - which we can't do with managed components.

Instead, they are cloned as submodules into this directory (where does not matter, because
main/idf_component.yml tells CMAKE where to find them).
