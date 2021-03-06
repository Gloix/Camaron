#-------------------------------------------------
#
# Project created by QtCreator 2012-08-13T19:35:01
#
#-------------------------------------------------

QT       += core gui
QT       += opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Camaron
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x -U__STRICT_ANSI__
#QMAKE_CXXFLAGS += -static-libstdc++

#DEFINES += GLEW_STATIC

SOURCES += main.cpp\
	visualizador.cpp \
    EvaluationStrategies/PolyhedronArea.cpp \
    EvaluationStrategies/PolygonArea.cpp \
    EvaluationStrategies/EvaluationStrategy.cpp \
    Factories/ModelLoadingFactory.cpp \
    Factories/PropertyFieldLoadingFactory.cpp \
    Model/VertexCloud.cpp \
    Model/PolyhedronMesh.cpp \
    Model/PolygonMesh.cpp \
    Model/Model.cpp \
    Model/Element/Vertex.cpp \
    Model/Element/Polyhedron.cpp \
    Model/Element/Polygon.cpp \
    Model/Element/Element.cpp \
    ModelLoading/ModelLoadingStrategy.cpp \
    ModelLoading/ModelLoadingOff.cpp \
    Rendering/Renderer.cpp \
    Rendering/OldSimpleRenderer.cpp \
    SelectionStrategies/SelectionStrategy.cpp \
    SelectionStrategies/Selection.cpp \
    UI/CustomGLViewer.cpp \
    Utils/Printer.cpp \
    Utils/PolygonUtils.cpp \
    Utils/shaderutils.cpp \
    Rendering/RModel/rvertex.cpp \
    Rendering/RModel/rmodel.cpp \
    Utils/matrixtransformationutils.cpp \
    Rendering/Renderers/ModelMainRenderer/mainrendererconfig.cpp \
    Rendering/Renderers/ModelMainRenderer/mainrenderer.cpp \
    Rendering/Renderers/PropertyValuesRenderer/propertyvaluesrendererconfig.cpp \
    Rendering/Renderers/PropertyValuesRenderer/propertyvaluesrenderer.cpp \
    Utils/qtutils.cpp \
    rendererconfigpopup.cpp \
    Factories/EvaluationStrategyRegistry.cpp \
    Factories/selectionstrategyregistry.cpp \
    SelectionStrategies/SelectionStrategyByProperty/selectionstrategybyproperty.cpp \
    SelectionStrategies/SelectionStrategyByProperty/selectionstrategybypropertyconfig.cpp \
    Utils/sortsearchutils.cpp \
    UI/staticsframe.cpp \
    UI/chartingwidget.cpp \
    Utils/crosstimer.cpp \
    Utils/chararrayscanner.cpp \
    UI/selectiontableview.cpp \
    Model/Element/triangle.cpp \
    EvaluationStrategies/trianglequalitymeasure1.cpp \
    EvaluationStrategies/triangleradiusratio.cpp \
    ModelLoading/modelloadingelenode.cpp \
    ModelLoading/modelloadingelenodelw.cpp \
    ModelLoading/modelloadingts.cpp \
    Rendering/Renderers/ShadingEffectsRender/shadingeffectsrender.cpp \
    Rendering/Renderers/ShadingEffectsRender/shadingeffectsrenderconfig.cpp \
    UI/mainconfiguration.cpp \
    Rendering/Renderers/NormalRenderer/normalrenderer.cpp \
    Rendering/Renderers/NormalRenderer/normalrendererconfig.cpp \
    Utils/hashingtree.cpp \
    ModelElementOperations/modelelementoperation.cpp \
    Utils/keyboardstate.cpp \
    Utils/renderinglock.cpp \
    Utils/openglutils.cpp \
    SelectionStrategies/MouseSelection/mouseselection.cpp \
    SelectionStrategies/MouseSelection/mouseselectionconfig.cpp \
    ModelExport/modelexportstrategy.cpp \
    ModelExport/modelexportoff.cpp \
    Factories/modelexportstrategyregistry.cpp \
    Utils/fileutils.cpp \
    ModelExport/modelexportelenode.cpp \
    Utils/polyhedronutils.cpp \
    Exceptions/exceptionmessage.cpp \
    Exceptions/unknownextensionexception.cpp \
    Exceptions/modelloadingexception.cpp \
    ModelExport/modelexportts.cpp \
    ModelLoading/modelloadingtri.cpp \
    EvaluationStrategies/polyhedronvolume.cpp \
    Utils/elementutils.cpp \
    ModelGeneralStatics/modelgeneralstaticscollector.cpp \
    UI/modelgeneralstaticswidget.cpp \
    Rendering/Renderers/VertexRenderer/vertexrenderer.cpp \
    Rendering/Renderers/VertexRenderer/vertexrendererconfig.cpp \
    SelectionStrategies/ChangeSelectionType/changeselectiontype.cpp \
    SelectionStrategies/ChangeSelectionType/changeselectiontypeconfig.cpp \
    EvaluationStrategies/polygonaspectratio.cpp \
    SelectionStrategies/ExpandToNeighbors/expandtoneighborsconfig.cpp \
    SelectionStrategies/ExpandToNeighbors/expandtoneighbors.cpp \
    SelectionStrategies/SelectById/selectbyid.cpp \
    SelectionStrategies/SelectById/selectbyidconfig.cpp \
    Utils/dynamicorderedcontainerwithmaxsize.cpp \
    UI/RenderElementsIds/renderelementsids.cpp \
    ModelLoading/modelloadingm3d.cpp \
    Model/Element/tetrahedron.cpp \
    Rendering/Renderers/ConvexGeometryIntersectionRenderer/convexgeometryintersectionrenderer.cpp \
    Rendering/Renderers/ConvexGeometryIntersectionRenderer/plane.cpp \
    Rendering/Renderers/ConvexGeometryIntersectionRenderer/convexgeometrydefinedbyplanes.cpp \
    Rendering/Renderers/ConvexGeometryIntersectionRenderer/convexgeometryintersectionrendererconfig.cpp \
    UI/genericconfigwidget.cpp \
    Rendering/Renderers/ConvexGeometryIntersectionRenderer/convexgeometryparser.cpp \
    Rendering/Renderers/ConvexGeometryIntersectionRenderer/genericconvexgeometry.cpp \
    Rendering/Renderers/ConvexGeometryIntersectionRenderer/sphericconvexgeometry.cpp \
    Rendering/Renderers/ConvexGeometryIntersectionRenderer/convexgeometryintersectionconfigselection.cpp \
    ModelLoading/modelloadingvisf.cpp \
    ModelExport/modelexportvisf.cpp \
    Factories/RendererRegistry.cpp \
    UI/rendererslist.cpp \
    Rendering/Renderers/GlassRenderer/glassrenderer.cpp \
    Rendering/Renderers/GlassRenderer/glassrendererconfig.cpp \
    Rendering/Renderers/ShaderCommonFunctions/phongconfig.cpp \
    Rendering/Renderers/ShaderCommonFunctions/elementdrawnoptionconfig.cpp \
    Rendering/Renderers/ShaderCommonFunctions/phong1dconfig.cpp \
    Utils/framebufferobjecthandle.cpp \
    Rendering/Renderers/DepthRenderer/depthrenderer.cpp \
    Rendering/Renderers/DepthRenderer/depthrendererconfig.cpp \
    Rendering/Renderers/nullrenderer.cpp \
    ModelLoading/modelloadingply.cpp \
    Utils/endianess.cpp \
    UI/aboutcamaron.cpp \
    EvaluationStrategies/minimumdihedralangle.cpp \
    EvaluationStrategies/maximumdihedralangle.cpp \
    Utils/userinputtranslator.cpp \
    Utils/userinputtranslationunit.cpp \
    UI/KeyshortcutConfig/keyshortcutconfiguration.cpp \
    UI/KeyshortcutConfig/keyshortcutconfigurationcategorywidget.cpp \
    UI/KeyshortcutConfig/keyshortcutconfigurationitemwidget.cpp \
    EvaluationStrategies/maximumsolidangle.cpp \
    EvaluationStrategies/minimumsolidangle.cpp \
    UI/clickableqlabel.cpp \
    UI/modelloadingprogressdialog.cpp \
    Rendering/Renderers/PhongPerPixel/phongperpixel.cpp \
    Rendering/Renderers/CartesianCoordinatesAxis/cartesiancoordinatesaxis.cpp \
    Rendering/Renderers/CartesianCoordinatesAxis/cartesiancoordinatesaxisconfig.cpp \
    ModelLoading/ModelLoadingWorkers/polygonpolygonneighborhoodworker.cpp \
    Utils/qtworkerbase.cpp \
    ModelLoading/ModelLoadingWorkers/calculatepolygonnormalsworker.cpp \
    ModelLoading/ModelLoadingWorkers/calculatevertexnormalsworker.cpp \
    ModelLoading/ModelLoadingWorkers/calculatepolyhedrongeocenterworker.cpp \
    ModelLoading/ModelLoadingWorkers/calculatevertexnormalslightweightmodelworker.cpp \
    Model/lightweightpolygonmesh.cpp \
    Model/lightweightvertexcloud.cpp \
    Model/lightweightpolyhedronmesh.cpp \
    Model/LWElements/lwelement.cpp \
    Model/LWElements/lwvertex.cpp \
    Model/LWElements/lwpolygon.cpp \
    Model/LWElements/lwpolyhedron.cpp \
    ModelLoading/modelloadingofflw.cpp \
    ModelLoading/modelloadingplylw.cpp \
    Rendering/Renderers/SceneHelpers.cpp \
    Rendering/Renderers/HeightRenderer/heightrendererconfig.cpp \
    Rendering/Renderers/HeightRenderer/heightrenderer.cpp \
    EvaluationStrategies/minimuminternalangle.cpp \
    EvaluationStrategies/maximuminternalangle.cpp \
    EvaluationStrategies/tetrahedronradiusedgeratio.cpp \
    EvaluationStrategies/polygonverticescount.cpp \
    EvaluationStrategies/polyhedronpolygonscount.cpp \
    Rendering/Renderers/IsolineRenderer/isolinerenderer.cpp \
    Rendering/Renderers/IsosurfaceRenderer/isosurfacerenderer.cpp \
    Rendering/Renderers/AdditionalEdgesRenderer/additionaledgesrenderer.cpp \
    Rendering/Renderers/ScalarPropertyRenderer/scalarpropertyrenderer.cpp \
    Rendering/Renderers/ScalarPropertyRenderer/scalarpropertyrendererconfig.cpp \
    Rendering/Renderers/baserendererconfig.cpp \
    PropertyFieldLoading/PropertyFieldLoadingStrategy.cpp \
    #PropertyFieldLoading/propertyfieldloadingtqs.cpp \
    PropertyFieldLoading/propertyfieldloadingply.cpp \
    PropertyFieldLoading/propertyfieldloadingelenode.cpp \
    Model/Element/edge.cpp \
    UI/propertyfieldloaddialog.cpp \
    PropertyFieldLoading/propertyfielddef.cpp \
    PropertyFieldLoading/scalarfielddef.cpp \
    PropertyFieldLoading/tqspropertyfielddef.cpp \
    Exceptions/propertydefnotfoundexception.cpp \
    PropertyFieldLoading/scalarfieldlistaddervisitor.cpp \
    Rendering/Renderers/stepsrendererconfig.cpp

HEADERS  += visualizador.h \
    Common/Constants.h \
    EvaluationStrategies/PolyhedronArea.h \
    EvaluationStrategies/PolygonArea.h \
    EvaluationStrategies/EvaluationStrategy.h \
    Factories/ModelLoadingFactory.h \
    Factories/PropertyFieldLoadingFactory.h \
    Model/VertexCloud.h \
    Model/PolyhedronMesh.h \
    Model/PolygonMesh.h \
    Model/Model.h \
    Model/Element/Vertex.h \
    Model/Element/Polyhedron.h \
    Model/Element/Polygon.h \
    Model/Element/Element.h \
    ModelLoading/ModelLoadingStrategy.h \
    ModelLoading/ModelLoadingOff.h \
    Rendering/Renderer.h \
    Rendering/OldSimpleRenderer.h \
    SelectionStrategies/SelectionStrategy.h \
    SelectionStrategies/Selection.h \
    UI/CustomGLViewer.h \
    Utils/Printer.h \
    Utils/PolygonUtils.h \
    Utils/shaderutils.h \
    Rendering/RModel/rvertex.h \
    Rendering/RModel/rmodel.h \
    Utils/matrixtransformationutils.h \
    Rendering/RModel/RVertexFlagAttribute.h \
    Rendering/Renderers/ModelMainRenderer/mainrendererconfig.h \
    Rendering/Renderers/ModelMainRenderer/mainrenderer.h \
    Rendering/Renderers/PropertyValuesRenderer/propertyvaluesrendererconfig.h \
    Rendering/Renderers/PropertyValuesRenderer/propertyvaluesrenderer.h \
    Utils/qtutils.h \
    rendererconfigpopup.h \
    RegistryTemplate.h \
    Factories/EvaluationStrategyRegistry.h \
    Factories/selectionstrategyregistry.h \
    SelectionStrategies/SelectionStrategyByProperty/selectionstrategybyproperty.h \
    SelectionStrategies/SelectionStrategyByProperty/selectionstrategybypropertyconfig.h \
    Utils/sortsearchutils.h \
    UI/staticsframe.h \
    UI/chartingwidget.h \
    Utils/crosstimer.h \
    Utils/chararrayscanner.h \
    UI/selectiontableview.h \
    Model/Element/triangle.h \
    EvaluationStrategies/trianglequalitymeasure1.h \
    EvaluationStrategies/triangleradiusratio.h \
    ModelLoading/modelloadingelenode.h \
    ModelLoading/modelloadingelenodelw.h \
    ModelLoading/modelloadingts.h \
    Rendering/Renderers/ShadingEffectsRender/shadingeffectsrender.h \
    Rendering/Renderers/ShadingEffectsRender/shadingeffectsrenderconfig.h \
    UI/mainconfiguration.h \
    Rendering/Renderers/NormalRenderer/normalrenderer.h \
    Rendering/Renderers/NormalRenderer/normalrendererconfig.h \
    Utils/hashingtree.h \
    ModelElementOperations/modelelementoperation.h \
    Utils/keyboardstate.h \
    Utils/renderinglock.h \
    Utils/openglutils.h \
    SelectionStrategies/MouseSelection/mouseselection.h \
    SelectionStrategies/MouseSelection/mouseselectionconfig.h \
    ModelExport/modelexportstrategy.h \
    ModelExport/modelexportoff.h \
    Factories/modelexportstrategyregistry.h \
    Utils/fileutils.h \
    ModelExport/modelexportelenode.h \
    Utils/polyhedronutils.h \
    Exceptions/exceptionmessage.h \
    Exceptions/unknownextensionexception.h \
    Exceptions/modelloadingexception.h \
    ModelExport/modelexportts.h \
    ModelLoading/modelloadingtri.h \
    EvaluationStrategies/polyhedronvolume.h \
    Utils/elementutils.h \
    ModelGeneralStatics/modelgeneralstaticscollector.h \
    UI/modelgeneralstaticswidget.h \
    Rendering/Renderers/VertexRenderer/vertexrenderer.h \
    Rendering/Renderers/VertexRenderer/vertexrendererconfig.h \
    SelectionStrategies/ChangeSelectionType/changeselectiontype.h \
    SelectionStrategies/ChangeSelectionType/changeselectiontypeconfig.h \
    EvaluationStrategies/polygonaspectratio.h \
    SelectionStrategies/ExpandToNeighbors/expandtoneighborsconfig.h \
    SelectionStrategies/ExpandToNeighbors/expandtoneighbors.h \
    Utils/lowmemoryhash.hpp \
    SelectionStrategies/SelectById/selectbyid.h \
    SelectionStrategies/SelectById/selectbyidconfig.h \
    Utils/dynamicorderedcontainerwithmaxsize.h \
    UI/RenderElementsIds/renderelementsids.h \
    ModelLoading/modelloadingm3d.h \
    Model/Element/tetrahedron.h \
    Rendering/Renderers/ConvexGeometryIntersectionRenderer/convexgeometryintersectionrenderer.h \
    Rendering/Renderers/ConvexGeometryIntersectionRenderer/plane.h \
    Rendering/Renderers/ConvexGeometryIntersectionRenderer/convexgeometrydefinedbyplanes.h \
    Rendering/Renderers/ConvexGeometryIntersectionRenderer/convexgeometryintersectionrendererconfig.h \
    UI/genericconfigwidget.h \
    Rendering/Renderers/ConvexGeometryIntersectionRenderer/convexgeometryparser.h \
    Rendering/Renderers/ConvexGeometryIntersectionRenderer/genericconvexgeometry.h \
    Rendering/Renderers/ConvexGeometryIntersectionRenderer/sphericconvexgeometry.h \
    Rendering/Renderers/ConvexGeometryIntersectionRenderer/convexgeometryintersectionconfigselection.h \
    ModelLoading/modelloadingvisf.h \
    ModelExport/modelexportvisf.h \
    Factories/RendererRegistry.h \
    UI/rendererslist.h \
    Rendering/Renderers/GlassRenderer/glassrenderer.h \
    Rendering/Renderers/GlassRenderer/glassrendererconfig.h \
    Rendering/Renderers/ShaderCommonFunctions/phongconfig.h \
    Rendering/Renderers/ShaderCommonFunctions/elementdrawnoptionconfig.h \
    Rendering/Renderers/ShaderCommonFunctions/phong1dconfig.h \
    Utils/framebufferobjecthandle.h \
    Rendering/Renderers/DepthRenderer/depthrenderer.h \
    Rendering/Renderers/DepthRenderer/depthrendererconfig.h \
    Rendering/Renderers/nullrenderer.h \
    ModelLoading/modelloadingply.h \
    Utils/endianess.h \
    UI/aboutcamaron.h \
    EvaluationStrategies/minimumdihedralangle.h \
    EvaluationStrategies/maximumdihedralangle.h \
    Utils/userinputtranslator.h \
    Utils/userinputtranslationunit.h \
    UI/KeyshortcutConfig/keyshortcutconfiguration.h \
    UI/KeyshortcutConfig/keyshortcutconfigurationcategorywidget.h \
    UI/KeyshortcutConfig/keyshortcutconfigurationitemwidget.h \
    EvaluationStrategies/maximumsolidangle.h \
    EvaluationStrategies/minimumsolidangle.h \
    UI/clickableqlabel.h \
    UI/modelloadingprogressdialog.h \
    Rendering/Renderers/PhongPerPixel/phongperpixel.h \
    Rendering/Renderers/CartesianCoordinatesAxis/cartesiancoordinatesaxis.h \
    Rendering/Renderers/CartesianCoordinatesAxis/cartesiancoordinatesaxisconfig.h \
    ModelLoading/ModelLoadingWorkers/polygonpolygonneighborhoodworker.h \
    Utils/qtworkerbase.h \
    ModelLoading/ModelLoadingWorkers/calculatepolygonnormalsworker.h \
    ModelLoading/ModelLoadingWorkers/calculatevertexnormalsworker.h \
    ModelLoading/ModelLoadingWorkers/calculatepolyhedrongeocenterworker.h \
    ModelLoading/ModelLoadingWorkers/fixsurfacepolygonsverticesorderworker.h \
    ModelLoading/ModelLoadingWorkers/calculatevertexnormalslightweightmodelworker.h \
    Model/lightweightpolygonmesh.h \
    Model/lightweightvertexcloud.h \
    Model/lightweightpolyhedronmesh.h \
    Model/LWElements/lwelement.h \
    Model/LWElements/lwvertex.h \
    Model/LWElements/lwpolygon.h \
    Model/LWElements/lwpolyhedron.h \
    ModelLoading/modelloadingofflw.h \
    ModelLoading/modelloadingplylw.h \
    Rendering/SimpleShaders/SimpleShader.h \
    Rendering/Renderers/SceneHelpers.h \
    Rendering/Renderers/HeightRenderer/heightrendererconfig.h \
    Rendering/Renderers/HeightRenderer/heightrenderer.h \
    Rendering/SimpleShaders/SimpleShader.h \
    EvaluationStrategies/minimuminternalangle.h \
    EvaluationStrategies/maximuminternalangle.h \
    EvaluationStrategies/tetrahedronradiusedgeratio.h \
    EvaluationStrategies/polygonverticescount.h \
    EvaluationStrategies/polyhedronpolygonscount.h \
    Rendering/Renderers/IsolineRenderer/isolinerenderer.h \
    Rendering/Renderers/IsosurfaceRenderer/isosurfacerenderer.h \
    Rendering/Renderers/AdditionalEdgesRenderer/additionaledgesrenderer.h \
    Rendering/Renderers/ScalarPropertyRenderer/scalarpropertyrenderer.h \
    Rendering/Renderers/ScalarPropertyRenderer/scalarpropertyrendererconfig.h \
    Rendering/Renderers/baserendererconfig.h \
    PropertyFieldLoading/PropertyFieldLoadingStrategy.h \
    #PropertyFieldLoading/propertyfieldloadingtqs.h \
    PropertyFieldLoading/propertyfieldloadingelenode.h \
    PropertyFieldLoading/propertyfieldloadingply.h \
    Model/Element/Edge.h \
    UI/propertyfieldloaddialog.h \
    PropertyFieldLoading/propertyfielddef.h \
    PropertyFieldLoading/scalarfielddef.h \
    PropertyFieldLoading/tqspropertyfielddef.h \
    Rendering/RModel/rmodelpropertyfielddef.h \
    Exceptions/propertydefnotfoundexception.h \
    Model/modelvisitor.h \
    PropertyFieldLoading/propertyfielddefvisitor.h \
    PropertyFieldLoading/scalarfieldlistaddervisitor.h \
    Rendering/Renderers/stepsrendererconfig.h

FORMS    += visualizador.ui \
    Rendering/Renderers/ModelMainRenderer/mainrendererconfig.ui \
    Rendering/Renderers/PropertyValuesRenderer/propertyvaluesrendererconfig.ui \
    rendererconfigpopup.ui \
    SelectionStrategies/SelectionStrategyByProperty/selectionstrategybypropertyconfig.ui \
    UI/staticsframe.ui \
    UI/selectiontableview.ui \
    Rendering/Renderers/ShadingEffectsRender/shadingeffectsrenderconfig.ui \
    UI/mainconfiguration.ui \
    Rendering/Renderers/NormalRenderer/normalrendererconfig.ui \
    SelectionStrategies/MouseSelection/mouseselectionconfig.ui \
    UI/modelgeneralstaticswidget.ui \
    Rendering/Renderers/VertexRenderer/vertexrendererconfig.ui \
    SelectionStrategies/ChangeSelectionType/changeselectiontypeconfig.ui \
    SelectionStrategies/ExpandToNeighbors/expandtoneighborsconfig.ui \
    SelectionStrategies/SelectById/selectbyidconfig.ui \
    UI/RenderElementsIds/renderelementsids.ui \
    Rendering/Renderers/ConvexGeometryIntersectionRenderer/convexgeometryintersectionrendererconfig.ui \
    Rendering/Renderers/ConvexGeometryIntersectionRenderer/convexgeometryintersectionconfigselection.ui \
    UI/rendererslist.ui \
    Rendering/Renderers/GlassRenderer/glassrendererconfig.ui \
    Rendering/Renderers/ShaderCommonFunctions/phongconfig.ui \
    Rendering/Renderers/ShaderCommonFunctions/elementdrawnoptionconfig.ui \
    Rendering/Renderers/ShaderCommonFunctions/phong1dconfig.ui \
    Rendering/Renderers/DepthRenderer/depthrendererconfig.ui \
    UI/aboutcamaron.ui \
    UI/KeyshortcutConfig/keyshortcutconfiguration.ui \
    UI/KeyshortcutConfig/keyshortcutconfigurationcategorywidget.ui \
    UI/KeyshortcutConfig/keyshortcutconfigurationitemwidget.ui \
    UI/modelloadingprogressdialog.ui \
    Rendering/Renderers/CartesianCoordinatesAxis/cartesiancoordinatesaxisconfig.ui \
    Rendering/Renderers/HeightRenderer/heightrendererconfig.ui \
    Rendering/Renderers/ScalarPropertyRenderer/scalarpropertyrendererconfig.ui \
    UI/propertyfieldloaddialog.ui \
    Rendering/Renderers/stepsrendererconfig.ui

OTHER_FILES += \
    Rendering/Renderers/ModelMainRenderer/mmr.vert \
    Rendering/Renderers/ModelMainRenderer/mmr.frag \
    Rendering/Renderers/PropertyValuesRenderer/pvr.frag \
    Rendering/Renderers/PropertyValuesRenderer/pvr.vert \
    Rendering/Renderers/ShaderCommonFunctions/phong.su \
    Rendering/Renderers/ModelMainRenderer/mmr.geom \
    Rendering/Renderers/PropertyValuesRenderer/pvr.geom \
    Rendering/Renderers/ShadingEffectsRender/ser.vert \
    Rendering/Renderers/ShadingEffectsRender/ser.frag \
    Rendering/Renderers/ShaderCommonFunctions/version400.su \
    Rendering/Renderers/NormalRenderer/nr.frag \
    Rendering/Renderers/NormalRenderer/nr.vert \
    Rendering/Renderers/NormalRenderer/nr.geom \
    SelectionStrategies/MouseSelection/ms.vert \
    SelectionStrategies/MouseSelection/ms.frag \
    Rendering/Renderers/VertexRenderer/vr.frag \
    Rendering/Renderers/VertexRenderer/vr.vert \
    SelectionStrategies/MouseSelection/ms.geom \
    UI/RenderElementsIds/reids.frag \
    UI/RenderElementsIds/reids.vert \
    UI/RenderElementsIds/reids.geom \
    UI/RenderElementsIds/numberTexture.raw \
    Rendering/Renderers/ConvexGeometryIntersectionRenderer/cgirmm.frag \
    Rendering/Renderers/ConvexGeometryIntersectionRenderer/cgirmm.vert \
    Rendering/Renderers/ConvexGeometryIntersectionRenderer/cgirmm.geom \
    Rendering/Renderers/ConvexGeometryIntersectionRenderer/cgircg.frag \
    Rendering/Renderers/ConvexGeometryIntersectionRenderer/cgircg.vert \
    Rendering/Renderers/ConvexGeometryIntersectionRenderer/intersectionFunctions.vert \
    Rendering/Renderers/GlassRenderer/grf.frag \
    Rendering/Renderers/GlassRenderer/grv.vert \
    Rendering/Renderers/ShaderCommonFunctions/phong1D.su \
    UI/CustomGLViewer/WeightedAverageShader.vert \
    UI/CustomGLViewer/WeightedAverageShader.frag \
    Rendering/Renderers/DepthRenderer/drf.frag \
    Rendering/Renderers/DepthRenderer/drbs.vert \
    Rendering/Renderers/DepthRenderer/drbs.frag \
    Rendering/Renderers/DepthRenderer/drv.vert \
    Rendering/Renderers/ModelMainRenderer/mmrdepthpeeling.frag \
    Rendering/Renderers/ModelMainRenderer/simpletexture.frag \
    Rendering/Renderers/ModelMainRenderer/simpletexture.vert \
    Rendering/Renderers/PhongPerPixel/ppp.vert \
    Rendering/Renderers/PhongPerPixel/ppp.frag \
    Rendering/Renderers/CartesianCoordinatesAxis/cca.vert \
    Rendering/Renderers/CartesianCoordinatesAxis/cca.frag \
    Rendering/Renderers/HeightRenderer/height_shader.vert \
    Rendering/Renderers/HeightRenderer/height_shader.frag \
    Rendering/Renderers/IsolineRenderer/ir.vert \
    Rendering/Renderers/IsolineRenderer/ir_gen.vert \
    Rendering/Renderers/IsolineRenderer/ir.frag \
    Rendering/Renderers/IsolineRenderer/ir_gen.geom \
    Rendering/Renderers/IsosurfaceRenderer/is.vert \
    Rendering/Renderers/IsosurfaceRenderer/is_gen.vert \
    Rendering/Renderers/IsosurfaceRenderer/is.frag \
    Rendering/Renderers/IsosurfaceRenderer/is_gen.geom \
    Rendering/Renderers/ScalarPropertyRenderer/spr.frag \
    Rendering/Renderers/ScalarPropertyRenderer/spr.vert \
    Rendering/Renderers/ScalarPropertyRenderer/spr.geom
RESOURCES += \
    images.qrc \
    shaders.qrc

win32 {
    INCLUDEPATH += $$PWD/libs/win32/glew/include
    INCLUDEPATH += $$PWD/libs/win32/glm
    LIBS += -Llib/libQt5OpenGL.a -lopengl32
    LIBS += $$PWD/libs/win32/glew/bin/Release/x64/glew32.dll
    LIBS += -Lglew32
}

unix {
    INCLUDEPATH += $$PWD/libs/unix/glew/include
    INCLUDEPATH += $$PWD/libs/unix/glm
    LIBS += -L$$PWD/libs/unix/glew/lib
    LIBS += -lGLEW
}
