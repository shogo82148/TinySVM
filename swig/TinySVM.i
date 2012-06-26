%module TinySVM

%{
#include <tinysvm.h>
#include <ctype.h>
#include <string.h>
%}

%ignore TinySVM::feature_node;
%ignore TinySVM::Param;

%ignore TinySVM::str2feature_node;
%ignore TinySVM::copy_feature_node;
%ignore TinySVM::fix_feature_node;

%ignore TinySVM::BaseExample::l;
%ignore TinySVM::BaseExample::d;
%ignore TinySVM::BaseExample::pack_d;
%ignore TinySVM::BaseExample::y;
%ignore TinySVM::BaseExample::x;
%ignore TinySVM::BaseExample::feature_type;
%ignore TinySVM::BaseExample::class_type;
%ignore TinySVM::BaseExample::alpha;
%ignore TinySVM::BaseExample::G;
%ignore TinySVM::BaseExample::svindex_size;
%ignore TinySVM::BaseExample::operator=(BaseExample&);

%extend TinySVM::BaseExample {
    double getY(int i)
    {
        if (i >= 0 && i < self->l) return self->y[i];
        fprintf(stderr, "BaseExample::getY() -- index is out of range\n");
        return 0.0;
    }

    char const * getX(int i)
    {
        const char *s = self->get(i);
        if (!s) return NULL;

        int len = strlen (s);
        for (i = 0; i < len;) {
            while (isspace (s[i])) i++;
            while (i < len && !isspace (s[i])) i++;
            while (i < len && isspace (s[i]))  i++;
            break;
        }

        return (const char*)(s + i);
    }

    double getAlpha(int i)
    {
        if (self->alpha && i >= 0 && i < self->svindex_size) return self->alpha[i];
        fprintf(stderr, "BaseExample::getAlpha() -- \nalpha is not available or index is out of range\n");
        return 0.0;
    }

    double getGradient(int i)
    {
        if (self->G && i >= 0 && i < self->svindex_size) return self->G[i];
        fprintf(stderr, "BaseExample::getGradient() -- \ngradient is not available or index is out of range\n");
        return 0.0;
    }

    double getG(int i)
    {
        if (self->G && i >= 0 && i < self->svindex_size) return self->G[i];
        fprintf(stderr, "BaseExample::getG() -- index is out of range\n");
        return 0.0;
    }
}

%feature("notabstract") TinySVM::Example;
%ignore TinySVM::Example::rebuildSVindex(Model *);
%ignore TinySVM::Example::learn(Param const *);

%extend TinySVM::Example {
    Model *learn()
    {
        return self->learn((char*)NULL);
    }
}


%feature("notabstract") TinySVM::Model;
%ignore TinySVM::Model::b;
%ignore TinySVM::Model::sphere;
%ignore TinySVM::Model::loss;
%ignore TinySVM::Model::bsv;
%ignore TinySVM::Model::operator=(Model&);


%include "../src/misc.h"
%include "../src/param.h"
%include "../src/base_example.h"
%include "../src/model.h"
%include "../src/example.h"
